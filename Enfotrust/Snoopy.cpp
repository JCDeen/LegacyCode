// Copyright © 2001 - 2005 enfoTrust networks, inc.

#pragma warning(disable:4786)

#include "stdafx.h"

#include <winbase.h>

#include "snoopy.h"
//#include "pace.h"
#include "../pace/questionnaire.h"
#include "../pace/call.h"

#include "error.h"
#include "version.h"

#include "../Acuity/Questionnaire.h"
#include "../Acuity/Question.h"
#include "../Acuity/Answer.h"
#include "../Acuity/XMLDocument.h"

/////////////////////////////////////////////////////////////////////////////
// These need to be defined for Release as well as Debug:                  //
// _UNICODE,UNICODE,_DEBUG,DEBUG,WIN32,_CONSOLE,HAVE_XML_ERROR,USE_LIBXML2 //
/////////////////////////////////////////////////////////////////////////////

//int GetAnswersData();
wstring GetAnswersData(int QuestionID);

//-----------------------------------------------------------------------------

static char fileversion[] = "$Header: /Source/Snoopy/Snoopy.cpp 2     4/07/04 8:20a Cdeen $";

//-----------------------------------------------------------------------------

CSnoopy::CSnoopy() : CService(SERVICE_DNAME_SNOOPY, SERVICE_LNAME_SNOOPY, FVERSION_S)
{
	m_lQueueThreadCount = 0;
}

//----------------------------------------------------------------------------

CSnoopy::~CSnoopy()
{
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::LoadRegistrySettings(CRegistrySettings & pRegistrySettings)
{
	SetServiceUser(SNOOPY_DEFAULT_USERNAME);
	SetServicePassword(SNOOPY_DEFAULT_PASSWORD);

	m_sCentralServer = pRegistrySettings.GetValue(SERVICE_REGISTRY_CENTRAL_SERVER,SERVICE_DEFAULT_CENTRAL_SERVER);
	m_sCentralDatabase = pRegistrySettings.GetValue(SERVICE_REGISTRY_CENTRAL_DATABASE,SERVICE_DEFAULT_CENTRAL_DATABASE);
	m_lPollingInterval = pRegistrySettings.GetValue(SERVICE_REGISTRY_POLLING_INTERVAL, SERVICE_DEFAULT_POLLING_INTERVAL);

	SetExtendedThreadCount();

	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::Initialize(int argc, LPCTSTR argv[])
{
	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::CheckAlreadyRunning()
{
	if ( !IsServerClustered() )
	{
		m_Semaphore = CreateSemaphore( NULL, 0, 1, SERVICE_DNAME_SNOOPY );
		if ( m_Semaphore == INVALID_HANDLE_VALUE )
		{
			LOGHERE (HERE, LOG_ERROR, L"CreateSemaphore failed: [%d],", GetLastError ());
			return HRESULT_FROM_WIN32 ( GetLastError ());
		}

		if ( GetLastError () == ERROR_ALREADY_EXISTS )
		{
			LOGHERE (HERE, LOG_ERROR,
							L"Another [%s] process is currently running. Only one [%s] process at a time can run.",
							m_sServiceName.c_str(), m_sServiceName.c_str() );

			return HRESULT_FROM_WIN32 ( ERROR_ALREADY_EXISTS );
		}
	}

	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::Run ( int argc, LPCTSTR argv[] )
{
	HRESULT hr;

	SERVICE_EVENT se;

	DWORD dwSinceLastChatter = ms ();

	CCommandLine cmd ( argc, argv );

	CDBInfo pDatabaseInfo;
	CDBInfo::DATABASE_INFO_LIST pDatabaseList;	// typdef vector<CDBInfo> DATABASE_INFO_LIST;

	CQueue pQueue;
	CQueue::QueueList pQueueList;

	CQueue::QueueList::iterator qli;
	CQueue::QueueVector::iterator qvi;

	SETWHO ( SERVICE_LNAME_SNOOPY );

	pDatabaseInfo.SetCentralUser(GetServiceUser(), GetServicePassword());
	pDatabaseInfo.SetCentralInfo(m_sCentralServer, m_sCentralDatabase);
	pDatabaseInfo.SetDatabaseInfo(m_sCentralServer, m_sCentralDatabase);

	pDatabaseList.push_back(pDatabaseInfo);		// 
	pDatabaseInfo.SetDatabaseList(pDatabaseList);

	pQueue.SetDatabaseInfo(pDatabaseInfo);

	hr = pQueue.Connect();
	RETIFERR(hr);

	// select the list of queues
	hr = pQueue.Select(SERVICE_SNAME_SNOOPY, m_lServiceInstance, pQueueList);
	RETIFERR(hr);

	hr = SetExtendedThreadCount(pQueueList.size());
	IFERR(hr)
	{
		LOGHERE ( HERE, LOG_ERROR, L"Failed to set extended thread count" );
		return hr;
	}

	// start the queue threads
	qli = pQueueList.begin();
	while ( qli != pQueueList.end() )
	{
		qvi = (*qli).second.begin();
		while ( qvi != (*qli).second.end() )
		{
			CServiceThreadAdaptor<CSnoopy, CQueue> pProcessFilesThreadAdaptor(this, &CSnoopy::ProcessQueue);
			pProcessFilesThreadAdaptor.SetParam( (*qvi) );
			pProcessFilesThreadAdaptor.SetName( (*qvi)->GetQueueName() );

			hr = m_pThreadPool.Start( pProcessFilesThreadAdaptor );
			IFERR(hr) {
				LOGHERE ( HERE, LOG_ERROR, L"Failed to start queue processing thread" );
				return hr;
			}

			qvi++;
		}

		qli++;
	}
	LOGHERE ( HERE, LOG_ALWAYS, L"SNOOPY::::::::::::: just started the queue threads :::::::::::");

	// start the maintenance loop
	while ( true )
	{
		LOGHERE ( HERE, LOG_DEBUG, L"SNOOPY::-------- inside maintenance loop : TOP ------");
		// check status of service event
		se = WaitForServiceEvent(SERVICE_DEFAULT_MAINTENANCE_INTERVAL);
		if ( se == SERVICE_EVENT_STOP || m_ss == SERVICE_EVENT_PUT_DOWN ) {

         	LOGHERE ( HERE, LOG_ALWAYS, L"[%s] service stop event signalled", m_sServiceName.c_str() );

			if ( m_ss == SERVICE_EVENT_PUT_DOWN ) {

              LOGHERE ( HERE, LOG_ALWAYS, L"[%s] put down", m_sServiceName.c_str() );
			  exit( m_ss );
			}

			break;
		}

		if ( (ms() - dwSinceLastChatter) > 3600000 ) // 1 hour
		{
			LOGHERE ( HERE, LOG_ALWAYS, L"[%s] service still running", m_sServiceName.c_str() );

			dwSinceLastChatter = ms ();

			CLog::CheckRollover();
		}
	}

	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::ProcessQueue(auto_ptr<CQueue> pQueue)
{
	HRESULT hr;

	SERVICE_EVENT se;

	long lConsecutiveErrors = 0;
	DWORD dwStartTime = ms ();

	CDBInfo pDatabaseInfo;
	CDBInfo::DATABASE_INFO_LIST pDatabaseList;

	LOGHERE ( HERE, LOG_DEBUG, L"SNOOPY::::::::::::: in ProcessQueue()" );

	while ( true )
	{
		SETWHO ( SERVICE_LNAME_SNOOPY );

		// check status of service event
		se = WaitForServiceEvent(m_lPollingInterval);
		if ( se == SERVICE_EVENT_STOP ) {
			break;
		}

		// create a new "shadow" queue that will contain the list
		// of polled records.
		auto_ptr<CQueue> pShadowQueue( new CQueue );
		if ( !pShadowQueue.get() )
		{
			LOGHERE ( HERE, LOG_ERROR, L"Failed to allocate queue entry list object" );
			ResetQueues();

			return E_OUTOFMEMORY;
		}

		// connect the queue object for polling
		hr = pQueue->Connect();
		RETIFERR(hr);

		// select a single connection for processing
		hr = pQueue->Select (*pShadowQueue);
		if ( hr == S_FALSE )
		{
			hr = S_OK;
			continue;
		}

		else if ( SUCCEEDED(hr) )
		{
			CServiceThreadAdaptor<CSnoopy, CQueue> pProcessFilesThreadAdaptor(this, &CSnoopy::ProcessFiles);
			pProcessFilesThreadAdaptor.SetParam( pShadowQueue );
			pProcessFilesThreadAdaptor.SetName( pShadowQueue->GetQueueName() );

			hr = m_pThreadPool.Start( pProcessFilesThreadAdaptor );
			IFERR(hr) {
				LOGHERE ( HERE, LOG_ERROR, L"Failed to start file processing thread" );
				continue;
			}

			lConsecutiveErrors = 0;
		}

		else if ( FAILED(hr) )
		{
			if ( lConsecutiveErrors++ >= 3 )
			{
				LOGHERE ( HERE, LOG_ERROR, L"Too many consecutive errors; SERVICE will exit." );

				// do a graceful shutdown here
                LOGHERE ( HERE, LOG_ERROR, L"PrivateShutdown() being called" );

                PrivateShutdown( (LPCWSTR*)m_sServiceName.c_str() );

				LOGHERE ( HERE, LOG_ERROR, L"PrivateShutdown() completed!" );

				m_ss = SERVICE_EVENT_PUT_DOWN;

				// Update/Reset the queues from 201 to 130
			    hr = pQueue->Update( ProcessingState, PendingState );
				IFERR(hr) {
				   LOGHERE (HERE, LOG_ERROR, L"Failed to update state for queue-entry [%s]", pQueue->GetQueueSQLName() );
                }

				break;
			}
			else {

                LOGHERE ( HERE, LOG_ERROR, L"Real Network error occurred. Sleeping for %d seconds.", SLEEP30 );
				Sleep( SLEEP30 );
			}
		}

		Sleep ( 1 );
	}

	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::ProcessFiles(auto_ptr<CQueue> pQueue)
{
	HRESULT hr;
	HRESULT hrInsertStatus = S_OK;	// Echos status of inserting the new file into d'base.

	SERVICE_EVENT se;

	CDBTile * pDTDatabase = NULL;	/// Do we need this for snoopy ? NO.But something similar.

	long lCalls, lQuestionnaires;
	lCalls = lQuestionnaires = 0;

	CQueueEntry pFirstEntry;
	CQueueEntry::QueueEntryList pQueueEntryList;
	CQueueEntry::QueueEntryList::iterator qei;

	SYSTEMTIME stSystemTime;

	DWORD dwProcessStartTime = ms();

	// connect to the queue server
	hr = pQueue->Connect();
	RETIFERR(hr);
	pQueueEntryList = pQueue->GetQueueEntryList();
	pFirstEntry = pQueueEntryList.front();

	// TO DO: //////////////////////////////////////////////////////////////
	// Should probably have a one time lookup of data servers here with   //
	// ptrs to the various databases to cut down on excessive lookup time //
	// by the stored procedure methods near end of this file.             //
	// ( Modify existing LookupDataServers() method. )                    //
	////////////////////////////////////////////////////////////////////////

	for (qei = pQueueEntryList.begin(); qei != pQueueEntryList.end(); qei++ )
	{
		wstring sFilename = (*qei).GetFileName();
		long lQueueEntryID = (*qei).GetEntryID();

		///////////////////////////////////////////////////////////
		// Set up some member vars for access by called methods. //
		///////////////////////////////////////////////////////////
		m_sInFilename = sFilename;
		m_sConnectionSID = (*qei).GetConnectionSID();
		m_sDeviceSID = (*qei).GetDeviceSID();

		// check status of service event
		se = WaitForServiceEvent(0);
		if ( se == SERVICE_EVENT_STOP )
		{
			SETWHO(SERVICE_LNAME_SNOOPY);

			LOGHERE ( HERE, LOG_PROGRESS,
								L"Detected service shutdown; resetting unprocessed queue entries for connection [%s]",
								(*qei).GetConnectionSID().c_str() );

			hr = pQueue->Update((*qei).GetConnectionSID(), ProcessingState, PendingState);
			RETIFERR(hr);

			break;
		}

		SETWHO ( (*qei).GetDeviceSID().c_str() );

		// make sure not already processed
		if ((*qei).GetStateID() == ProcessedState ) 
		{
			continue; // Fall thru to end of loop
		}

		LOGHERE ( HERE, LOG_PROGRESS,
							L"Processing queue-entry [%d: %s]",
							(*qei).GetEntryID(),
							(*qei).GetFileName().c_str(),
							(*qei).GetConnectionSID().c_str() );

		if ( (*qei).GetBuffer().GetBufferLen() == 0 )
		{
			LOGHERE ( HERE, LOG_ERROR, L"Queue entry [%d] data is zero-length; skipping", (*qei).GetEntryID() );
			(*qei).SetStateID(TruncatedState);
			hr = pQueue->Update ((*qei));
			IFERR(hr) {
				LOGHERE (HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
			}
			continue; // Fall thru to end of loop
		}

		////////////////////////
		// Process CALL files //
		////////////////////////
		if ( (sFilename.find(L"call.acp") != wstring::npos)			 // Make sure its a CALL file
			&& (sFilename.find(L"questionnaire") == wstring::npos)) // and NOT a QUESTIONNAIRE file
		{
			////////////////////////////////////////////////
			// We're really just using the Call class     //
			// to parse the XML in this Call.ACP file.    //
			// (We can do it *right* after the deadline.) //
			////////////////////////////////////////////////
			DWORD dwCallProcStartTime = ms(); // <---------------------------- REMOVE ???
			CCall pCall(NULL, NULL);

			// set processed timestamp
//			GetLocalTime(&stSystemTime);
			GetSystemTime(&stSystemTime);

			(*qei).SetProcessedDateTime (stSystemTime);

			/////////////////////////////////////
			// decrypt and uncompress the file //
			/////////////////////////////////////
			hr = (*qei).DecryptAndUnzip();
			IFERR(hr)
			{
				LOGHERE ( HERE, LOG_ERROR, L"Failed to decompress/decrypt file [%s]", sFilename.c_str() );
				(*qei).SetStateID (FileErrorState);
				hr = pQueue->Update( (*qei) );
				IFERR(hr) {
					LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
				}
				continue;
			}


			pCall.SetConnectionSID( (*qei).GetConnectionSID() ); // <--- Do we need this?
			pCall.SetBuffer( (*qei).GetBuffer() );	 // Copies XML text from queue into Call class.

			wstring wsConnectionSID = (*qei).GetConnectionSID();

			////////////////////////////////////////////////////////////////
			// Parses the Call file data from XML into CCall member vars. //
			// (this is really the only reason we are using CCall class.) //
			////////////////////////////////////////////////////////////////
			hr = pCall.LoadFromXML();

			if ( FAILED(hr) )
			{
				LOGHERE ( HERE, LOG_ERROR, L"Failed to load file data for file [%s]", sFilename.c_str() );
				(*qei).SetStateID (DataErrorState);
				hr = pQueue->Update( (*qei) );
				IFERR(hr) 
				{
					LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
				}
				continue; // Fall thru to end of loop
			}
			else // XML was loaded from Call file OK.
			{
				//////////////////////////
				// Look up the StoreID. //
				//////////////////////////
				m_sStoreNumber = pCall.GetStoreNumber();
//				wstring wsStoreID;
				hr = LookupStoreID(m_sStoreID, m_sStoreNumber); // Need to get this from XML !!!

				///////////////////////////
				// If StoreID not found. //
				///////////////////////////
				if (hr)
				{
					LOGHERE ( HERE, LOG_ERROR, L"SNOOPY:::::StoreID not found.");

					/////////////////////////////////////////////////////////
					// Date/Time will be generated by stored procedure, so //
					// we only need to pass in MAC ID and Queue Entry ID   //
					/////////////////////////////////////////////////////////
					hr = InsertIntoQueue_ACP_920(m_sStoreNumber, lQueueEntryID);

					(*qei).SetStateID (ServerErrorState); // Set Queue entry state to 920	
					hr = pQueue->Update( (*qei) );
					
					IFERR(hr) 
						LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );

					//////////////////////////////////////////////////
					// Since StoreID not found in database Snoopy's //
					// processing of this file is complete.         //
					//////////////////////////////////////////////////
					continue; // Fall thru to end of loop
				}
				else 
				{
					///////////////////////////////////////////////////////////////////////////
					// StoreID WAS found, so replace -1 in StoreID XML with looked up value. //
					///////////////////////////////////////////////////////////////////////////
					LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY:::::StoreID WAS found.");

					CBuffer oldBuffer;					// incoming Call file XML data (blob).
					CBuffer newBuffer;					// outgoing Call file XML data (blob).
					oldBuffer.Copy(pCall.GetBuffer());	// Make copy of existing XML blob.

					// Find positions of the <StoreID> XML tags.

					const char * pszTagStart = "<StoreID>";	// Just to make coding easier.
					const char * pszTagStop = "</StoreID>"; // Just to make coding easier.
					int nPosStoreIDTagStart = oldBuffer.Scan((const PBYTE) pszTagStart, 0);

					///////////////////////////////////////////////////////
					// Copy data up to & including end of <StoreID> tag. //
					///////////////////////////////////////////////////////
					newBuffer.Write(oldBuffer.GetBuffer(0), nPosStoreIDTagStart);

					////////////////////////////////////////////////
					// Write the looked up StoreID into XML text. //
					////////////////////////////////////////////////
					newBuffer.Write(m_sStoreID.c_str());

					//////////////////////////////////////////////////
					// Copy data from and including </StoreID> tag. //
					//////////////////////////////////////////////////
					int nPosStoreIDTagEnd = oldBuffer.Scan((const PBYTE) pszTagStop, 0);
					int nLastHalfSize = oldBuffer.GetBufferLen() - nPosStoreIDTagEnd;
					newBuffer.Write(oldBuffer.GetBuffer(nPosStoreIDTagEnd - strlen(pszTagStop)), nLastHalfSize + strlen(pszTagStop)); // Copy 2nd part.

					////////////////////////////////////////////////////////////////
					// Write newly modified blob to RR_Upload_Blobs. Also renames //
					// file extension to ".xml", and sets status of new file.     //
					////////////////////////////////////////////////////////////////
					hr = Insert_into_RR_Upload_Files(newBuffer);
					if (!hr)
					{
						///////////////////////////////////////////////
						// Call.ACP file was processed successfully. //
						///////////////////////////////////////////////
						(*qei).SetStateID (ProcessedState); // ProcessedState = enum 202.
						hr = pQueue->Update( (*qei) );
						IFERR(hr)
						{
							LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
						}
					}
					else
						LOGHERE ( HERE, LOG_ERROR, L"Failed to create new RR_Uploadfiles entry for queue-entry [%d]", (*qei).GetEntryID() );
				}
			}

			lCalls++;
		}

		////////////////////////////////////
		// // Process QUESTIONNAIRE files //
		////////////////////////////////////
		else if ((sFilename.find(L"questionnaire ") != wstring::npos) ||	// Make sure its a QUESTIONNAIRE file
		         (sFilename.find(L"questionnaire") != wstring::npos))	// Make sure its a QUESTIONNAIRE file
		{
			CQuestionnaire pQuestionnaire(NULL, NULL);

			// set processed timestamp
//			GetLocalTime(&stSystemTime);
			GetSystemTime(&stSystemTime);

			(*qei).SetProcessedDateTime (stSystemTime);

			/////////////// NOTE: decryption & decompression code used to be here.///////////////
			/////////////////////////////////////
			// decrypt and uncompress the file //
			/////////////////////////////////////
			hr = (*qei).DecryptAndUnzip();
			IFERR(hr)
			{
				LOGHERE ( HERE, LOG_ERROR, L"Failed to decompress/decrypt file [%s]", sFilename.c_str() );
				(*qei).SetStateID (FileErrorState);
				hr = pQueue->Update( (*qei) );
				IFERR(hr) {
					LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
				}
				continue;
			}

			pQuestionnaire.SetConnectionSID( (*qei).GetConnectionSID() );
			pQuestionnaire.SetBuffer( (*qei).GetBuffer() ); // // Copies XML text from queue into CQuestionnaire class.


			//////////////////////////////////////////////////////////////////////////////////
			// Parses the Questionnaire file data from XML into CQuestionnaire member vars. //
			// (this is really the only reason we are using CQuestionnaire class.)          //
			//////////////////////////////////////////////////////////////////////////////////
			hr = pQuestionnaire.LoadFromXML();

			if ( FAILED(hr) )
			{
				LOGHERE ( HERE, LOG_ERROR, L"Failed to load file data for file [%s]", sFilename.c_str() );
				(*qei).SetStateID (DataErrorState);
				hr = pQueue->Update( (*qei) );
				IFERR(hr) {
					LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
				}
				continue; // Fall thru to end of loop
			}
			else // XML was loaded from Questionnaire file OK.
			{
				///////////////////////////////////////////////////////////////
				// Read StoreID from questionnaire file. It BETTER be there! //
				///////////////////////////////////////////////////////////////
//				wstring wsStoreID;
				long lStoreID = pQuestionnaire.GetStoreID();

				TCHAR bufferStoreID[100];
				_ltow(lStoreID, bufferStoreID, 10);
				m_sStoreID = bufferStoreID;

				///////////////////////////////////////////////////////////////
				// Write Questionnaire blob to RR_Upload_Blobs. Also renames //
				// file extension to ".xml", and sets status of new file.    //
				///////////////////////////////////////////////////////////////
				hr = Insert_into_RR_Upload_Files(pQuestionnaire.GetBuffer());

				if (!hr)
				{
					////////////////////////////////////////////////////////
					// Questionnaire.ACP file was processed successfully. //
					////////////////////////////////////////////////////////
					(*qei).SetStateID (ProcessedState); // ProcessedState = enum 202.
					hr = pQueue->Update( (*qei) );
					IFERR(hr)
					{
						LOGHERE ( HERE, LOG_ERROR, L"Failed to update state for queue-entry [%d]", (*qei).GetEntryID() );
					}
				}
				else
					LOGHERE ( HERE, LOG_ERROR, L"Failed to create new RR_Uploadfiles entry for queue-entry [%d]", (*qei).GetEntryID() );

				XMLDocument xmlDoc(pQuestionnaire.GetBuffer());
				Questionnaire q;
				((XMLReader *) &q)->processDocument(xmlDoc.root().getChild().sibling());
				const QuestionList &ql = q.getQuestionList();

				m_sPoleFoundation		= ql.getQuestionByID(33).getAnswerList().answerAt(0).getString();
				m_sPoleBase				= ql.getQuestionByID(34).getAnswerList().answerAt(0).getString();
				m_sPoleElectricService	= ql.getQuestionByID(35).getAnswerList().answerAt(0).getString();	
				m_sFixtureMountHeight	= ql.getQuestionByID(36).getAnswerList().answerAt(0).getString();	
				m_sFixtureDescription	= ql.getQuestionByID(37).getAnswerList().answerAt(0).getString();	
				m_sNbrofFixtures		= ql.getQuestionByID(39).getAnswerList().answerAt(0).getString();	
				m_sPoleTrafficArms		= ql.getQuestionByID(40).getAnswerList().answerAt(0).getString();
				m_sBracketLength		= ql.getQuestionByID(41).getAnswerList().answerAt(0).getString();	
				m_sLampType				= ql.getQuestionByID(42).getAnswerList().answerAt(0).getString();
				m_sLampWattage			= ql.getQuestionByID(43).getAnswerList().answerAt(0).getString();
				m_sPoleMaterial			= ql.getQuestionByID(44).getAnswerList().answerAt(0).getString();
				m_sPoleColor			= ql.getQuestionByID(45).getAnswerList().answerAt(0).getString();
				m_sPoleColor			= ql.getQuestionByID(45).getAnswerList().answerAt(0).getString();
				m_sPoleUsage			= ql.getQuestionByID(46).getAnswerList().answerAt(0).getString();

				hr = ENFOFACILITIESUpdate();
				if (hr)
					LOGHERE ( HERE, LOG_ERROR, L"Failed to update ENFOFACILITIES table queue-entry [%d]", (*qei).GetEntryID() );
				else
					LOGHERE ( HERE, LOG_PROGRESS, L"Successful Update of ENFOFACILITIES table for queue-entry [%d]", (*qei).GetEntryID() );

				////////////////////////////////////////////////////
				//// This code to be implemented AFTER demo ... ////
				////////////////////////////////////////////////////
				#ifdef USE_FOR_DEMO
				if (!m_sLongitude.empty() && !m_sLatitude.empty())
				{
					int hr = UpdateAcuityNodesData(lStoreID, m_sLatitude, m_sLongitude);
					if (hr)
						LOGHERE ( HERE, LOG_ERROR, L"Failed to update ROAMNodes queue-entry [%d]", (*qei).GetEntryID() );
				}
				#endif // USE_FOR_DEMO
			}
			lQuestionnaires++;
		}
		else // ignore unknown file-type
		{
			LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY::::Unknown File Type [%s]", sFilename.c_str());
			continue;
		}
	}

	CHECKDELETE(pDTDatabase);

	LOGHERE ( HERE, LOG_ALWAYS,
						L"Processed: [%d] queue-entries in [%d] milliseconds",
						lCalls + lQuestionnaires,
						ms() - dwProcessStartTime);

	return S_OK;
}

//----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////
// To Do:                                                    // 
//		Modify this to return pointers to                    //
//		ACQCentral, ACQrover, & acqgis_data database servers.//
///////////////////////////////////////////////////////////////
HRESULT CSnoopy::LookupDataServers(const wstring & sDeviceSID, CDBTile ** pDTDatabase)
{
	HRESULT hr;
	HRESULT lhr = S_OK;

	CDBInfo pDatabaseInfo;

	pDatabaseInfo.SetCentralInfo( m_sCentralServer, m_sCentralDatabase );
	pDatabaseInfo.SetCentralUser( GetServiceUser(), GetServicePassword() );

	if ( (*pDTDatabase) == NULL )
	{
		// lookup and connect to data-tile
		LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY::::::: ABOUT TO LOOK UP %s", sDeviceSID.c_str() );

		lhr = pDatabaseInfo.LookupDataServerByDeviceSID ( DATABASE_TYPE_DATATILE, sDeviceSID.c_str() );
		IFERR(lhr)
		{
			switch ( HRESULT_CODE(lhr) )
			{
				case ERROR_SERVER_DISABLED:
					LOGHERE ( HERE, LOG_WARNING,
								L"Database [DATABASE_TYPE_DATATILE] is disabled' ignoring" );
					break;

				default:
					LOGHERE ( HERE, LOG_ERROR,
								L"Failed to lookup [DATABASE_TYPE_DATATILE]; queue entries will be ignored" );
					break;
			}
		}
		else
		{
			(*pDTDatabase) = new CDBTile;
			if ( (*pDTDatabase) == NULL )
			{
				RETERR( HERE,
							L"Failed to allocate [DATATILE] database object",
							HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY) );
			}

			hr = (*pDTDatabase)->Connect( pDatabaseInfo );
			IFERR(hr)
			{
				CHECKDELETE((*pDTDatabase));
				return hr;
			}

			hr = (*pDTDatabase)->Initialize();
			IFERR(hr)
			{
				CHECKDELETE((*pDTDatabase));
				return hr;
			}
		}
	}

	return (FAILED(lhr) ? lhr : S_OK);
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::SetExtendedThreadCount(long lExtendedThreadCount)
{
	long lThreadPoolMaxCount = m_pThreadPool.GetMax();

	// set the service specific thread count
	if ( m_lQueueThreadCount == 0 ) {
		m_lQueueThreadCount = lExtendedThreadCount;
	}

	// increase the size of the thread pool
	if ( lThreadPoolMaxCount != SERVICE_DEFAULT_THREAD_POOL_MAX )
	{
		if ( m_lQueueThreadCount != 0 )
		{
			LOGHERE ( HERE, LOG_ALWAYS,
								L"Expanding size of service thread pool from [%d] to [%d]",
								lThreadPoolMaxCount, lThreadPoolMaxCount + m_lQueueThreadCount );

			// set themax threads; this is the service specific count plus
			// the current count
			m_pThreadPool.SetMax( lThreadPoolMaxCount + m_lQueueThreadCount );
		}
	}

	return S_OK;
}

//----------------------------------------------------------------------------

HRESULT CSnoopy::ResetQueues() {

	// Connect to the database and reset the queue
	LOGHERE ( HERE, LOG_ALWAYS, L"SNOOPY:: Resetting the Queue ...");

	HRESULT hr;

	CDBInfo pDatabaseInfo;
	CDBInfo::DATABASE_INFO_LIST pDatabaseList;

	CQueue pQueue;

	SETWHO ( SERVICE_LNAME_SNOOPY ); // Sets pszWho in CLog 

	pDatabaseInfo.SetCentralUser(GetServiceUser(), GetServicePassword() );
	pDatabaseInfo.SetCentralInfo(m_sCentralServer, m_sCentralDatabase);
	pDatabaseInfo.SetDatabaseInfo(m_sCentralServer, m_sCentralDatabase);
	pDatabaseList.push_back(pDatabaseInfo);
	pDatabaseInfo.SetDatabaseList(pDatabaseList);

	pQueue.SetDatabaseInfo(pDatabaseInfo);

	hr = pQueue.Connect();
	RETIFERR(hr);

	hr = pQueue.ResetQue( GetServiceUser(), GetServicePassword(),
		                  SERVICE_SNAME_SNOOPY, m_lServiceInstance,
						  ProcessingState, PendingState );
	RETIFERR(hr);

	return S_OK;
}

/////////////////////////////////////////////////////////////////
// Looks up the StoreID keyed on StoreNumber from Stores table.//
// Uses stored procedure "GetStoreIDFromStores" in ACQCentral. //
// Gets StoreID into wstring parameter instead of Long parm.   //
/////////////////////////////////////////////////////////////////
HRESULT CSnoopy::LookupStoreID(wstring &wsStoreID, wstring wsStoreNumber)
{
	long l_StoreID = 0;
	int hr = LookupStoreID(l_StoreID, wsStoreNumber);

	TCHAR buffer[100];
	_ltow(l_StoreID, buffer, 10);

	wsStoreID = buffer;

	return hr;
}

/////////////////////////////////////////////////////////////////
// Looks up the StoreID keyed on StoreNumber from Stores table.//
// Uses stored procedure "GetStoreIDFromStores" in ACQCentral. //
/////////////////////////////////////////////////////////////////
HRESULT CSnoopy::LookupStoreID(long &lStoreID, wstring wsStoreNumber)
{
	CDBSession pSession;
	int hr  = pSession.Connect(m_sCentralServer, m_sCentralDatabase, GetServiceUser(), GetServicePassword());

	if (!hr) // If no error
	{
		LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY:::ACQCentral Connect Status = %d", hr);

		CDBCommand pCommand;

		hr = !hr ? pCommand.SetSession(&pSession) : hr;
		hr = !hr ? pCommand.SetStoredProcedure (L"GetStoreIDFromStores", 1, 1, 1) : hr;
		hr = !hr ? pCommand.DefineLongParameter (0, L"@ReturnValue", eDBReturnValue) : hr;        
		hr = !hr ? pCommand.DefineStringParameter (1, L"@StoreNumber") : hr;        
		hr = !hr ? pCommand.DefineLongColumn(0, L"@StoreID") : hr;        
		hr = !hr ? pCommand.Prepare() : hr;
		hr = !hr ? pCommand.Clear() : hr;
		hr = !hr ? pCommand.SetStringParameter(1, wsStoreNumber) : hr;
		hr = !hr ? pCommand.Execute() : hr;
		hr = !hr ? pCommand.GetLongColumn(0, lStoreID) : hr;
		/////////////////////////////////////////////////////
		// The procedure returns a 0 if StoreID not found. //
		// According to DBA, Enfotrust always starts a key //
		// column with "1", so we use "0" to test for err. //
		/////////////////////////////////////////////////////
		if (lStoreID == 0)
			hr = E_FAIL;

		if (hr)
			LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::ERROR Getting StoreID");
	}
	else
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::Could NOT Connect To ACQCentral");

	return hr;
}

HRESULT CSnoopy::ENFOFACILITIESUpdate()
{
	int hr = S_OK;

	CDBInfo pDatabaseInfo;
	pDatabaseInfo.SetCentralInfo( m_sCentralServer, m_sCentralDatabase );
	pDatabaseInfo.SetCentralUser( GetServiceUser(), GetServicePassword() );

	CDBSession pSession;
	hr = pSession.Connect( L"10.10.6.102", L"acqgis_data", GetServiceUser(), GetServicePassword());

	if (!hr) // If no error
	{
		LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY:::ACQGis Connect Status = %d", hr);

		CDBCommand pCommand;
		hr = !hr ? pCommand.SetSession(&pSession) : hr;

		hr = !hr ? pCommand.SetStoredProcedure (L"ENFOFACILITIESUpdate", 1, 14, 0) : hr;
		hr = !hr ? pCommand.DefineLongParameter (0, L"@ReturnValue", eDBReturnValue) : hr;

		hr = !hr ? pCommand.DefineStringParameter ( 1, L"@NodeID") : hr; // Really is NodeID        
		hr = !hr ? pCommand.DefineStringParameter ( 2, L"@PoleFoundation") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 3, L"@PoleBase") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 4, L"@PoleElectricService") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 5, L"@FixtureMountHeight") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 6, L"@FixtureDescription") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 7, L"@NbrofFixtures") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 8, L"@PoleTrafficArms") : hr;        
		hr = !hr ? pCommand.DefineStringParameter ( 9, L"@BracketLength") : hr;
		hr = !hr ? pCommand.DefineStringParameter (10, L"@LampType") : hr;        
		hr = !hr ? pCommand.DefineStringParameter (11, L"@LampWattage") : hr;        
		hr = !hr ? pCommand.DefineStringParameter (12, L"@PoleMaterial") : hr;
		hr = !hr ? pCommand.DefineStringParameter (13, L"@PoleColor") : hr;
		hr = !hr ? pCommand.DefineStringParameter (14, L"@PoleUsage") : hr;

		hr = !hr ? pCommand.Prepare() : hr;
		hr = !hr ? pCommand.Clear() : hr;

		hr = !hr ? pCommand.SetStringParameter ( 1, m_sStoreID) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 2, m_sPoleFoundation) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 3, m_sPoleBase) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 4, m_sPoleElectricService) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 5, m_sFixtureMountHeight) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 6, m_sFixtureDescription) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 7, m_sNbrofFixtures) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 8, m_sPoleTrafficArms) : hr;        
		hr = !hr ? pCommand.SetStringParameter ( 9, m_sBracketLength) : hr;
		hr = !hr ? pCommand.SetStringParameter (10, m_sLampType) : hr;        
		hr = !hr ? pCommand.SetStringParameter (11, m_sLampWattage) : hr;        
		hr = !hr ? pCommand.SetStringParameter (12, m_sPoleMaterial) : hr;
		hr = !hr ? pCommand.SetStringParameter (13, m_sPoleColor) : hr;
		hr = !hr ? pCommand.SetStringParameter (14, m_sPoleUsage) : hr;

		hr = !hr ? pCommand.Execute() : hr;

		if (hr)
			LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::ERROR updating ENFOFACILITIES table");
	}
	else
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::Could NOT Connect To ACQGis");

	return hr;
}

////////////////////////////////////////////////////////////////////////////////////
// Snoopy only needs to update this data if it is present in the Questionnaire. ? //
////////////////////////////////////////////////////////////////////////////////////
// MACID should go away ...       
//// StoredProc name may change ...
///// HRESULT CSnoopy::UpdateROAMNodesData(long lNodeID, wstring wsMACID, wstring wsLatitude, wstring wsLongitude)
HRESULT CSnoopy::UpdateAcuityNodesData(long lNodeID, wstring wsLatitude, wstring wsLongitude)
{

#define DEMO_ONLY
#ifdef DEMO_ONLY
	return S_OK;
#endif
	
	int hr = S_OK;
	CDBInfo pDatabaseInfo;
	pDatabaseInfo.SetCentralInfo( m_sCentralServer, m_sCentralDatabase );
	pDatabaseInfo.SetCentralUser( GetServiceUser(), GetServicePassword() );

	CDBSession pSession;
	hr = pSession.Connect( L"10.10.6.102", L"acqgis_data", GetServiceUser(), GetServicePassword());

	if (!hr) // If no error
	{
		LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY:::ACQGis Connect Status = %d", hr);

		CDBCommand pCommand;
		hr = !hr ? pCommand.SetSession(&pSession) : hr;

		hr = !hr ? pCommand.SetStoredProcedure (L"AcuityNodesUpdate", 0, 3, 0) : hr;
		hr = !hr ? pCommand.DefineLongParameter (0, L"@ReturnValue", eDBReturnValue) : hr;
        
		hr = !hr ? pCommand.DefineLongParameter   (0, L"@NodeID") : hr;        
		hr = !hr ? pCommand.DefineStringParameter (1, L"@Latitude") : hr;        
		hr = !hr ? pCommand.DefineStringParameter (2, L"@Longitude") : hr;        
		hr = !hr ? pCommand.Prepare() : hr;
		hr = !hr ? pCommand.Clear() : hr;

		hr = !hr ? pCommand.SetLongParameter  (0, lNodeID) : hr;
		hr = !hr ? pCommand.SetStringParameter(1, wsLatitude) : hr;
		hr = !hr ? pCommand.SetStringParameter(2, wsLongitude) : hr;
		hr = !hr ? pCommand.Execute() : hr;

		if (hr)
			LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::ERROR executing stored proc AcuityNodesUpdate");
	}
	else
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY::::Could NOT Connect To ACQGis");

	return hr;
}


//////////////////////////////////////////////////////////////
// Inserts MAC ID and queue entry into Queue_ACP_920 table. //
//////////////////////////////////////////////////////////////
HRESULT CSnoopy::InsertIntoQueue_ACP_920(wstring wsMac_ID, long lQueueEntryID)
{
	CDBSession pSession;
	int hr  = pSession.Connect(m_sCentralServer, m_sCentralDatabase, GetServiceUser(), GetServicePassword() );

	if (!hr) // If no error
	{
		LOGHERE ( HERE, LOG_PROGRESS, L"SNOOPY:::ACQCentral Connect Status = %d", hr);

		CDBCommand pCommand;

		hr = !hr ? pCommand.SetSession(&pSession) : hr;
		hr = !hr ? pCommand.SetStoredProcedure (L"QueueACP920Insert", 0, 2, 0) : hr;
		hr = !hr ? pCommand.DefineStringParameter (0, L"@MacID") : hr;        
		hr = !hr ? pCommand.DefineLongParameter (1, L"@EntryID") : hr; 
		hr = !hr ? pCommand.Prepare() : hr;
		hr = !hr ? pCommand.Clear() : hr;
		hr = !hr ? pCommand.SetStringParameter(0, wsMac_ID) : hr;
		hr = !hr ? pCommand.SetLongParameter(1, lQueueEntryID) : hr;
		hr = !hr ? pCommand.Execute() : hr;
	}
	else
	{
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY:::::: Could NOT Connect To ACQCentral");
		hr = E_FAIL;
	}

	if (hr)
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY:::Trouble Inserting into QueueACP920");

	return hr;
}


///////////////////////////////////////////////////////////////////////////////////
// Insert_into_RR_Upload_Files() does the following:                             //
//		Renames ".acp" filename to ".xml" .                                      // 
//		Inserts a new blob in RR_UploadBlobs (stored procedure does this first). //
//		Inserts a new entry into RR_UploadFiles (stored proc does this next).    //
//		Sets state of this new entry to ReceivingState so Triage will process it.//
///////////////////////////////////////////////////////////////////////////////////
HRESULT CSnoopy::Insert_into_RR_Upload_Files(CBuffer xmlBuffer)
{
	HRESULT hr;
	long lReturnCode = 0;
	wstring sFileName;
//	long lStateID = 202;

	/////////////////////////////////////////////////////////////////////////
	// Replace .ACP extension with .XML . If for some reason we can't do   //
	// this, we return early since rest of file processing doesn't matter. //
	// If this catch() is executed, suspect serious errs like memory leaks //
	// elsewhere in code before you get to this point.                     //
	// NOTE: Leave setting of queue status to the code calling this method.//
	/////////////////////////////////////////////////////////////////////////
	wstring wsNewFilename = m_sInFilename;
	try
	{
		int nPosExt = wsNewFilename.find_last_of(L".acp"); // Search from rear of string.
		wsNewFilename.replace(nPosExt - 3, 4, L".xml");
	}
	catch (...)
	{
		LOGHERE ( HERE, LOG_ERROR, L"Could not rename file:%S", m_sInFilename );
		return E_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////////
	// If file rename was successful, we can continue with the rest of the job. //
	// I use the funky "hr = !hr ?" syntax to facilitate stepping in debugger.  //
	// It prevents later functions from being executed if the former had errors.//
	//////////////////////////////////////////////////////////////////////////////
	CDBInfo pDatabaseInfo;
	pDatabaseInfo.SetCentralInfo(m_sCentralServer, m_sCentralDatabase );
	pDatabaseInfo.SetCentralUser(GetServiceUser(), GetServicePassword());

	CDBSession pSession;
	hr = pSession.Connect(L"10.10.6.102", L"ACQRedRover", GetServiceUser(), GetServicePassword());

	CDBCommand pFileInsertCmd;
	hr = !hr ? pFileInsertCmd.SetSession (&pSession) : hr;
	hr = !hr ? pFileInsertCmd.SetStoredProcedure (L"usp_gs_upload_file_insert", 1, 11, 1) : hr;

	hr = !hr ? pFileInsertCmd.DefineLongParameter  (0, L"@ReturnValue", eDBReturnValue) : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(1, L"@ConnectionSID", 50) : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(2, L"@DeviceSID", 50) : hr;
	hr = !hr ? pFileInsertCmd.DefineLongParameter  (3, L"@StateID") : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(4, L"@FileName", 255) : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(5, L"@FileTime", 50) : hr;
	hr = !hr ? pFileInsertCmd.DefineLongParameter  (6, L"@FileSize") : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(7, L"@FileType", 255) : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(8, L"@ReceivedTime", 50) : hr;
	hr = !hr ? pFileInsertCmd.DefineStringParameter(9, L"@ProcessedTime", 50) : hr;
	hr = !hr ? pFileInsertCmd.DefineLongParameter  (10, L"@TransferTime") : hr;
	hr = !hr ? pFileInsertCmd.DefineBlobParameter  (11, L"@FileData") : hr;
	
	hr = !hr ? pFileInsertCmd.DefineLongColumn(0, L"@BlobID") : hr;
	hr = !hr ? pFileInsertCmd.Prepare () : hr;
	hr = !hr ? pFileInsertCmd.Clear() : hr;

	/////////////////////////////////////////////////////////////////////////////
	// Get the timestamp for right now, when this new data is being pumped out.//
	/////////////////////////////////////////////////////////////////////////////
	SYSTEMTIME stSystemTime;
//	GetLocalTime(&stSystemTime);
	GetSystemTime(&stSystemTime);
	WCHAR szTimestamp[MAX_PATH];
	wstring wsTimestampNow = CreateTimeStamp(szTimestamp, stSystemTime);

	hr = !hr ? pFileInsertCmd.SetStringParameter( 1, m_sConnectionSID) : hr; // From incoming .ACP
	hr = !hr ? pFileInsertCmd.SetStringParameter( 2, m_sDeviceSID) : hr;		// From incoming .ACP
	hr = !hr ? pFileInsertCmd.SetLongParameter	( 3, ReceivingState) : hr;
	hr = !hr ? pFileInsertCmd.SetStringParameter( 4, wsNewFilename) : hr;
	hr = !hr ? pFileInsertCmd.SetStringParameter( 5, wsTimestampNow) : hr; // FileTime is file's last modified time.
	hr = !hr ? pFileInsertCmd.SetLongParameter	( 6, xmlBuffer.GetBufferLen()) : hr; // sizeof new XML
	hr = !hr ? pFileInsertCmd.SetStringParameter( 7, L"") : hr;				// FileType = NULL for now.
	hr = !hr ? pFileInsertCmd.SetStringParameter( 8, wsTimestampNow) : hr;	// Received time is when blob placed in database.
	hr = !hr ? pFileInsertCmd.SetStringParameter( 9, L"") : hr;	// Set by Triage after we make this entry.
	hr = !hr ? pFileInsertCmd.SetLongParameter	(10, 0) : hr;	// 0 for this exercise, since no PDA involved.
	hr = !hr ? pFileInsertCmd.SetBlobParameter	(11, xmlBuffer.GetBuffer(), xmlBuffer.GetBufferLen()) : hr;

	hr = !hr ? pFileInsertCmd.Execute() : hr;
	hr = !hr ? pFileInsertCmd.GetLongParameter(0, lReturnCode) : hr;

	if ((lReturnCode != 0) || hr)
	{
		LOGHERE ( HERE, LOG_ERROR, L"File insert command failed with code: [%d]", lReturnCode );
		return E_FAIL;
	}

	long m_lFileID = 0;
	hr = pFileInsertCmd.GetLongColumn(0, m_lFileID);
	LOGHERE ( HERE, LOG_PROGRESS, L"File insert completed for FileID=%d", m_lFileID );

	hr = Update_RR_Connections(); // Reset the state in RR_Connections so the new files get processed.

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
// In our case, the connection already has been inserted by the PDA sync.   //
// Snoopy is just resetting its state so that it will be processed again    //
// with the new .XML files instead of the old .ACP files we just processed. //
//////////////////////////////////////////////////////////////////////////////
HRESULT CSnoopy::Update_RR_Connections()
{
	HRESULT hr;
	long lReturnCode = 0;

	//////////////////////////////////////
	// Get the timestamp for right now. //
	//////////////////////////////////////
	SYSTEMTIME stSystemTime;
//	GetLocalTime(&stSystemTime);
	GetSystemTime(&stSystemTime);
	WCHAR szTimestamp[MAX_PATH];
	wstring wsTimestampNow = CreateTimeStamp(szTimestamp, stSystemTime);

	CDBSession pSession;
	hr = pSession.Connect(L"10.10.6.102", L"ACQRedRover", GetServiceUser(), GetServicePassword());

	CDBCommand pConnectionUpdateCmd;
	hr = !hr ? pConnectionUpdateCmd.SetSession(&pSession) : hr;
	hr = !hr ? pConnectionUpdateCmd.SetStoredProcedure (L"usp_gs_connection_update", 1, 3, 0) : hr;
	hr = !hr ? pConnectionUpdateCmd.DefineLongParameter (0, L"@ReturnValue", eDBReturnValue) : hr;
	hr = !hr ? pConnectionUpdateCmd.DefineStringParameter (1, L"@ConnectionSID", 50) : hr;
	hr = !hr ? pConnectionUpdateCmd.DefineLongParameter (2, L"@StateID") : hr;
	hr = !hr ? pConnectionUpdateCmd.DefineStringParameter (3, L"@StopTime", 50) : hr;
	hr = !hr ? pConnectionUpdateCmd.Prepare() : hr;

	pConnectionUpdateCmd.Clear();

	hr = !hr ? pConnectionUpdateCmd.SetStringParameter (1, m_sConnectionSID) : hr;
	hr = !hr ? pConnectionUpdateCmd.SetLongParameter (2, CompletedState) : hr;
	hr = !hr ? pConnectionUpdateCmd.SetStringParameter (3, wsTimestampNow) : hr;
	hr = !hr ? pConnectionUpdateCmd.Execute() : hr;

	IFERR ( hr )
	{
		LOGHERE ( HERE, LOG_ERROR, L"Error updating connection status [%0.8x]", hr );
		return hr;
	}

	hr = !hr ? pConnectionUpdateCmd.GetLongParameter(0, lReturnCode) : hr;

	if ((lReturnCode != 0) || hr)
	{
		LOGHERE ( HERE, LOG_ERROR, L"Connections Reset command failed with code: [%d]", lReturnCode );
		return E_FAIL;
	}

	LOGHERE ( HERE, LOG_PROGRESS, L"RR_Connections reset successfully.");
	return hr;
}

///////////////////////////////////////////////////////////
// This was a brute force method of parsing a buffer     //
// for the start and stop tags. Not true XML processing. //
///////////////////////////////////////////////////////////
wstring CSnoopy::FastLoadFromXML(CBuffer xmlBuffer, char* szStartTag, char* szEndTag)
{
	HRESULT hr = S_OK;
	WCHAR wcharString[1001];

	/////////////////////////////////
	// Get the Longitude from XML. //
	/////////////////////////////////
	try
	{
		int nPosXMLTagStart = xmlBuffer.Scan((const PBYTE) szStartTag, 0);
		int nPosXMLTagEnd = xmlBuffer.Scan((const PBYTE) szEndTag, 0);

		CBuffer cbufferXML;
		cbufferXML.Write(xmlBuffer.GetBuffer(nPosXMLTagStart), nPosXMLTagEnd - nPosXMLTagStart - strlen(szEndTag));
		mbstowcs(wcharString, (const char*) cbufferXML.GetBuffer(), strlen((const char*) cbufferXML.GetBuffer()) + 1 );
		return wcharString;
	}
	catch(...) // Fail gracefully.
	{
		LOGHERE ( HERE, LOG_ERROR, L"SNOOPY:::exception converting XML: %s", szStartTag);
		return L""; //hr = E_FAIL;
	}

}


/////////////////////////////////////////////////////////////
// These quick code hacks are here just to verify how some //
// legacy code works. Feel free to get rid of this stuff ! //
/////////////////////////////////////////////////////////////
HRESULT CSnoopy::MyTest(int &refValPtr)
{
	refValPtr = 1099;
	return S_OK;
}

HRESULT CSnoopy::MyTest()
{
	HRESULT hr = S_OK;
	wstring wsFileName = L"somefile.acp";
	int nPosExt = wsFileName.find(L".acp");
	wsFileName.replace(nPosExt, 4, L".xml");

	int nxDummyline = 0;

/*	long lReturnCode = 0;
	wstring sFileName;
	long lStateID = 202;

	CDBInfo pDatabaseInfo;
	pDatabaseInfo.SetCentralInfo( m_sCentralServer, m_sCentralDatabase );
	pDatabaseInfo.SetCentralUser( GetServiceUser(), GetServicePassword() );

	CDBSession pSession;
	int hr1 = pSession.Connect( L"10.10.6.102", L"ACQRedRover", L"snoopy", L"snoopy");

	CDBCommand pFileInsertCmd;

	hr = pFileInsertCmd.SetSession (&pSession);

	hr = pFileInsertCmd.SetStoredProcedure (L"usp_gs_upload_file_insert", 1, 11, 1);

	hr = pFileInsertCmd.DefineLongParameter	 (0, L"@ReturnValue", eDBReturnValue);
	hr = pFileInsertCmd.DefineStringParameter(1, L"@ConnectionSID", 50);
	hr = pFileInsertCmd.DefineStringParameter(2, L"@DeviceSID", 50);
	hr = pFileInsertCmd.DefineLongParameter	 (3, L"@StateID");
	hr = pFileInsertCmd.DefineStringParameter(4, L"@FileName", 255);
	hr = pFileInsertCmd.DefineStringParameter(5, L"@FileTime", 50);
	hr = pFileInsertCmd.DefineLongParameter	 (6, L"@FileSize");
	hr = pFileInsertCmd.DefineStringParameter(7, L"@FileType", 255);
	hr = pFileInsertCmd.DefineStringParameter(8, L"@ReceivedTime", 50);
	hr = pFileInsertCmd.DefineStringParameter(9, L"@ProcessedTime", 50);
	hr = pFileInsertCmd.DefineLongParameter  (10, L"@TransferTime");
	hr = pFileInsertCmd.DefineBlobParameter  (11, L"@FileData");
	
	hr = pFileInsertCmd.DefineLongColumn(0, L"@BlobID");
	hr = pFileInsertCmd.Prepare ();
	pFileInsertCmd.Clear();

	// Rename the .ACP to .XML //

	CBuffer myBuffer;
	myBuffer.Insert((const PBYTE) L"SNOOPY TEST\0", 11);

	hr = pFileInsertCmd.SetStringParameter	( 1, L"SNOOPYTEST"); // From incoming .ACP
	hr = pFileInsertCmd.SetStringParameter	( 2, L"SNOOPYTEST");	  // From incoming .ACP
	hr = pFileInsertCmd.SetLongParameter	( 3, ProcessedState);
	hr = pFileInsertCmd.SetStringParameter	( 4, L"SNOOP");
	hr = pFileInsertCmd.SetStringParameter	( 5, L"2005-09-23 15:39:39.000");		// FileTime is file's last modified time.
	hr = pFileInsertCmd.SetLongParameter	( 6, myBuffer.GetBufferLen()); // m_lFileSize);
	hr = pFileInsertCmd.SetStringParameter	( 7, L"ACP");			// FileType = NULL for now.
	hr = pFileInsertCmd.SetStringParameter	( 8, L"2005-09-23 15:39:39.000");		// Received time is when blob placed in database.
	hr = pFileInsertCmd.SetStringParameter	( 9, L"2005-09-23 15:39:39.000"); // wsProcessedDateTime);  // Time when Triage picked up the file;
	hr = pFileInsertCmd.SetLongParameter	(10, 0);	// 0 for this exercise, since no PDA involved.
	hr = pFileInsertCmd.SetBlobParameter	(11, myBuffer.GetBuffer(), myBuffer.GetBufferLen());

	hr = pFileInsertCmd.Execute ();
	hr = pFileInsertCmd.GetLongParameter(0, lReturnCode);

	if ( lReturnCode != 0 )
	{
		LOGHERE ( HERE, LOG_ERROR, L"File insert command failed with code: [%d]", lReturnCode );
		return E_FAIL;
	}

	long m_lFileID;
	hr = pFileInsertCmd.GetLongColumn(0, m_lFileID);
*/
	return S_OK;
}


HRESULT CSnoopy::MyCBufferTest()
{
	char szBuffer[] = "The Road to Riches Lies in Wait";
	CBuffer buffer;
	buffer.Insert((const PBYTE) szBuffer, sizeof(szBuffer));
	buffer.SaveToFile(L"MY_TESTFILE.TXT");
	return S_OK;
}

wstring GetAnswersData(int QuestionID)
{
	switch (QuestionID)
	{
		case 1:		// FACILITYID
			//return // value
			break;

		case 2:		// Status
			//return // value
			break;

		case 3:		// City
			//return // value
			break;

		case 4:		// InstallDate
			//return // value
			break;

		case 5:		// Address
			//return // value
			break;

		case 6:		// InstallDate
			//return // value
			break;

		case 7:		// Address
			//return // value
			break;

		case 8:		// FixtureType
			//return // value
			break;

		case 9:		// Lumens
			//return // value
			break;

		case 10:	// Watts
			//return // value
			break;

		case 11:	// PoleDescription
			//return // value
			break;

		case 12:	// MountDescription
			//return // value
			break;

		case 13:	// FixtureDescription
			//return // value
			break;

		case 14:	// XCoord
			//return // value
			break;

		case 15:	// YCoord
			//return // value
			break;
	}

/*	!m_sFACILITYID.empty() &&         
	!m_sStatus.empty() &&              
	!m_sCity.empty() &&                
	!m_sInstallDate.empty() &&     
	!m_sAddress.empty() &&        
	!m_sFixtureType.empty() &&     
	!m_sLumens.empty() &&              
	!m_sWatts.empty() &&               
	!m_sPoleDescription.empty() && 
	!m_sMountDescription.empty() &&    
	!m_sFixtureDescription.empty() &&  
	!m_sXCoord.empty() &&              
	!m_sYCoord.empty() )             
*/
//	return 0;
	return L"asdfasf";
}


