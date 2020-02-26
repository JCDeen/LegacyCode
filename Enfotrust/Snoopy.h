// Copyright © 2001 - 2005 enfoTrust networks, inc.

#ifndef __SNOOPY_H__
#define __SNOOPY_H__

#include "shared.h"
#include "service.h"
#include "dblib.h"
#include "database.h"
#include "queue.h"

//-----------------------------------------------------------------------------

#include <vector>
#include <memory>
#include <string>
using namespace std;

//----------------------------------------------------------------------------

#define	SERVICE_REGISTRY_CENTRAL_SERVER			(L"Central Database Server")
#define	SERVICE_REGISTRY_CENTRAL_DATABASE		(L"Central Database")
#define	SERVICE_REGISTRY_POLLING_INTERVAL		(L"Queue Polling Interval")

//----------------------------------------------------------------------------

#define SERVICE_DEFAULT_CENTRAL_SERVER			(L"<server>")
#define	SERVICE_DEFAULT_CENTRAL_DATABASE		(L"<database>")

#define	SERVICE_DEFAULT_POLLING_INTERVAL		(1000)
#define	SERVICE_DEFAULT_MAINTENANCE_INTERVAL	(30000)

//----------------------------------------------------------------------------

//#define	SNOOPY_DEFAULT_USERNAME				(L"Snoopy")
//#define	SNOOPY_DEFAULT_PASSWORD				(L"Snoopy")
#define	SNOOPY_DEFAULT_USERNAME				(L"snoopy")
#define	SNOOPY_DEFAULT_PASSWORD				(L"snoopy")

//----------------------------------------------------------------------------

class CSnoopy : public CService
{
	public:

		CSnoopy();
		~CSnoopy();

	public:

		virtual HRESULT LoadRegistrySettings(CRegistrySettings & pRegistrySettings);

		virtual HRESULT Initialize(int argc, LPCTSTR argv[]);
		virtual HRESULT Run(int argc, LPCTSTR argv[]);

		virtual HRESULT CheckAlreadyRunning();

		virtual HRESULT ProcessQueue(auto_ptr<CQueue> pQueue);
		virtual HRESULT ProcessFiles(auto_ptr<CQueue> pQueue);

		virtual HRESULT ResetQueues();

		HRESULT LookupStoreID(wstring &wsStoreID, wstring wsStoreNumber);
		HRESULT LookupStoreID(long &lStoreID, wstring wsStoreNumber);

		HRESULT UpdateAcuityNodesData(long lNodeID, wstring wsLatitude, wstring wsLongitude);
		HRESULT InsertIntoQueue_ACP_920(wstring wsMAC_ID, long lQueueEntryID);
		HRESULT Insert_into_RR_Upload_Files(CBuffer cBuff);
		HRESULT Update_RR_Connections();
		wstring FastLoadFromXML(CBuffer xmlBuffer, char* szStartTag, char* szEndTag);
		HRESULT ENFOFACILITIESUpdate();

		HRESULT MyTest(int &refValPtr);
		HRESULT MyTest();
		HRESULT MyCBufferTest();

	public:
		wstring m_sCentralServer;
		wstring m_sCentralDatabase;

		CBuffer m_xmlBuffer;

		wstring m_sConnectionSID;
		wstring m_sDeviceSID;
		wstring m_sInFilename;

		////////////////////////////////////////
		// Call and Questionnaire sheet data. //
		////////////////////////////////////////
		wstring m_sStoreID;
		wstring m_sStoreNumber;

		///////////////////////////////
		// Questionnaire sheet data. //
		///////////////////////////////
		wstring m_sPoleFoundation		;		
		wstring m_sPoleBase				;
		wstring m_sPoleElectricService	;
		wstring m_sFixtureMountHeight	;
		wstring m_sFixtureDescription	;
		wstring m_sNbrofFixtures		;
		wstring m_sPoleTrafficArms		;
		wstring m_sBracketLength		;
		wstring m_sLampType				;
		wstring m_sLampWattage			;
		wstring m_sPoleMaterial			;
		wstring m_sPoleColor			;
		wstring m_sPoleUsage			;
		
	private:

		HRESULT LookupDataServers(const wstring & sDeviceSID, CDBTile ** pDTDatabase);
		HRESULT SetExtendedThreadCount(long lExtendedThreadCount = 0);

	private:

		long	m_lPollingInterval;

		long	m_lTriggerState;
		long	m_lSuccessState;
		long	m_lErrorState;

		long	m_lQueueThreadCount;

		HANDLE	m_Semaphore;
};

//----------------------------------------------------------------------------

#endif
