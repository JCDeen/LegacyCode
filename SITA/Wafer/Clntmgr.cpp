#include "Clntmgr.hpp"

#ifdef _WIN32
#include "WFCSWin.h"
#else
#include "WFCSUnix.h"
#endif

ClientManager* gpClientMgr;

// ----------------------------------------------------------------------------
// Function name	: ClientManager::ClientManager
// Description	    : Constructor for a Client Manager
// Return type		: None.
// Argument         : long lTimeout
//                    The lTimeout value specifies the inactivity time for
//                    all clients.
// ----------------------------------------------------------------------------
ClientManager::ClientManager(long lTimeout)
{
#ifdef _WIN32
	m_pCriticalSection = new WFCS_Win32;
#else
	m_pCriticalSection = new WFCS_Unix;
#endif
	m_lTimeout = lTimeout;
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::~ClientManager
// Description	    : Destructor for the Client Manager.
//                    All the clients in the collection are terminated and
//                    the collection itself is destroyed.
// Return type		: None
// ----------------------------------------------------------------------------
ClientManager::~ClientManager()
{
	m_pCriticalSection->Lock();
	{
		long nCount = m_ClientCollection.Count();
		WaferClientInfo*	pWaferClientInfo;
		WaferClient*		pWaferClient;

		cout << "Terminating all WaferClients on shutdown" << endl;
		for (long i = 0; i < nCount; i++) {
			pWaferClientInfo = m_ClientCollection[0];
			pWaferClient = pWaferClientInfo->pWaferClient;

			m_ClientCollection.RemoveAtIndex(0);

			pWaferClient->Terminate();
			delete pWaferClient;
			delete pWaferClientInfo;
		}
	}
	m_pCriticalSection->UnLock();
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::Initialize
// Description	    : Initialize the Client Manager. Typically the
//                    monitor thread is started in this method.
// Return type		: TRUE if Monitor thread was successfully created, else
//                    FALSE.
// ----------------------------------------------------------------------------
ClientManager::Initialize()
{
#ifdef _WIN32
	gpClientMgr = this;

	DWORD dwThreadID;

	if (CreateThread (NULL, 0, ClientManager::BeginMonitorWaferClients, NULL, 0, &dwThreadID) == NULL) {
		cout << "CreateThread (ClientManager::MonitorWaferClients) failed. Reason -> " << GetLastError() << endl;
		return FALSE;
	}
	return TRUE;
#endif
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::CreateClient
// Description	    : This function creates a reference to an external client
//                    by creating a WaferClient object.
// Return type		: WaferClient* - The actual WaferClient object created.
//                    NULL if failure to create a WaferClient.
// Argument         : WaferSocket* pSocket - The socket associated with the
//                                           external client
// Argument         : WIP_RequestType eRT - The current request type.
// Argument         : WIP_ClientType eClientType - Type of the external client.
//                    It could be CGI/UserInterface or a Java applet.
// ----------------------------------------------------------------------------
WaferClient* ClientManager::CreateClient(WaferSocket* pSocket, 
										 WIP_RequestType eRT,
										 WIP_ClientType eClientType)
{
	WaferClient*		pWaferClient = NULL;
	WaferClientInfo*	pWaferClientInfo;

	m_pCriticalSection->Lock();
	{
		pWaferClient = new WaferClient(pSocket, eRT, eClientType);
	
		if (pWaferClient->Initialize() == FALSE) {
			// Unable to Initialize the WaferClient
			delete pWaferClient;

			pWaferClient = NULL;
		}
		else {
			char* szID = GenerateClientID();
			pWaferClient->SetClientID(szID);

			pWaferClientInfo = new WaferClientInfo;
			pWaferClientInfo->pWaferClient = pWaferClient;

			m_ClientCollection.Add(szID, pWaferClientInfo);

			delete [] szID;
		}
	}
	m_pCriticalSection->UnLock();
	
	return pWaferClient;
}


// ----------------------------------------------------------------------------
// Function name	: ClientManager::SetClientActive
// Description	    : This function sets the status of client to active/inactive
// Return type		: None
// Argument         : WaferClient* pWaferClient - The client object
// Argument         : ChildID_t tID - The ChildID of the client object. The
//                                    ChildID is the thread ID or the process ID
// Argument         : BOOL bActive - TRUE or FALSE
// ----------------------------------------------------------------------------
void ClientManager::SetClientActive(WaferClient* pWaferClient,
									ChildID_t tID, BOOL bActive)
{
	m_pCriticalSection->Lock();
	{
		pWaferClient->SetActive(bActive);
		pWaferClient->SetChildID(tID);
	}
	m_pCriticalSection->UnLock();
}


// ----------------------------------------------------------------------------
// Function name	: ClientManager::VerifyClient
// Description	    : Given the client ID, this function returns the associated
//                    WaferClient object, by searching the client collection.
// Return type		: WaferClient* - If the client ID is found in the collection
//                                   the associated WaferClient is returned.
//                                   Else NULL. Also a check is made to see if
//                                   a client with the same ID is still active.
//                                   If found, returns NULL once again.
// Argument         : string szClientID - The client ID to be found in the
//                                        collection.
// ----------------------------------------------------------------------------
WaferClient* ClientManager::VerifyClient(string szClientID)
{
	WaferClient* pWaferClient;
	WaferClientInfo* pWaferClientInfo;

	cout << "Trying to find ID <" << szClientID << "> in the collection" << endl;

	m_pCriticalSection->Lock();
	{
		pWaferClientInfo = m_ClientCollection.Find(szClientID);
	}
	m_pCriticalSection->UnLock();

	if (pWaferClientInfo == NULL) {
		cout << "ID <" << szClientID << "> not found" << endl;
		return NULL;
	}

	cout << "ID <" << szClientID << "> Found" << endl;
	pWaferClient = pWaferClientInfo->pWaferClient;

	// See if the same WaferClient id is already in use...
	if (pWaferClient->IsActive() == TRUE) {
		cout << "ID <" << szClientID << "> is active" << endl;
		return NULL;
	}

	return pWaferClient;
}


// ----------------------------------------------------------------------------
// Function name	: ClientManager::EndClient
// Description	    : Used only when a thread or a process could not created
//                    and the WaferClient object has to be deleted.
// Return type		: void 
// Argument         : string szClientID - The ID of the WaferClient.
// ----------------------------------------------------------------------------
void ClientManager::EndClient(string szClientID)
{
	WaferClientInfo* pWaferClientInfo;
	WaferClient* pWaferClient;

	m_pCriticalSection->Lock();
	{
		pWaferClientInfo = m_ClientCollection.Find(szClientID);

		if (pWaferClientInfo == NULL) {
			// It is a fatal internal error
		}
		else {
			pWaferClient = pWaferClientInfo->pWaferClient;
			m_ClientCollection.Remove(szClientID);

			delete pWaferClient;
			delete pWaferClientInfo;
		}
	}
	m_pCriticalSection->UnLock();
}


// ----------------------------------------------------------------------------
// Function name	: ClientManager::SetCompleted
// Description	    : When we know the ChildID of the WaferClient that has
//                    just terminated, this function is used to find the
//                    item in the collection and end it.
// Return type		: void 
// Argument         : ChildID_t tChildID - The ChildID of the WaferClient.
// Argument         : BOOL bKeepAlive - Flag to indicate if the client object
//                                      is to be kept alive or removed from
//                                      the collection.
// ----------------------------------------------------------------------------
void ClientManager::SetCompleted(ChildID_t tChildID, BOOL bKeepAlive)
{
	m_pCriticalSection->Lock();
	{
		long i;
		long nCount = m_ClientCollection.Count();
		WaferClientInfo* pWaferClientInfo;
		WaferClient* pWaferClient;

		for (i = 0; i < nCount; i++) {
			pWaferClientInfo = m_ClientCollection[i];
			pWaferClient = pWaferClientInfo->pWaferClient;

			if (pWaferClient->GetChildID() == tChildID)
				break;
		}

		if (i == nCount) {
			cout << "Fatal Error. Cannot find the ChildID in the Collection" << endl;
		}
		else {
			pWaferClient->SetInActive();
			pWaferClient->SetChildID(0);

			if (bKeepAlive == FALSE) {
				m_ClientCollection.RemoveAtIndex(i);
				cout << "Removed Client <" << pWaferClient->GetClientID() << "> from the collection" << endl;
				delete pWaferClient;
				delete pWaferClientInfo;
			}
			else {
				time(&pWaferClientInfo->lInactiveTime);
			}
		}
	}
	m_pCriticalSection->UnLock();
}


// ----------------------------------------------------------------------------
// Function name	: ClientManager::GetCount
// Description	    : Returns the number of items in the collection
// Return type		: long - Number of WaferClients in the collection
// ----------------------------------------------------------------------------
long ClientManager::GetCount()
{
	long lCount;

	m_pCriticalSection->Lock();
	{
		lCount = m_ClientCollection.Count();
	}
	m_pCriticalSection->UnLock();

	return lCount;
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::GenerateClientID
// Description	    : This is a simple ID Generating routine, that uses the
//                    current time to create a ClientID. Time specified in
//                    long is converted to alphabets.
// Return type		: char* - The generated ID.
// ----------------------------------------------------------------------------
char* ClientManager::GenerateClientID()
{
	struct timeb	stTimeB;
	char			*szID;
	char			szTemp1[256];
	char			szTemp2[256];
	int				nSeed;

	ftime(&stTimeB);

	sprintf(szTemp1, "%0.9d", stTimeB.time);
	sprintf(szTemp2, "%0.3d", stTimeB.millitm);

	szID = new char [13];

	sprintf(szID, "%.9s%.3s", szTemp1, szTemp2);

	nSeed = stTimeB.millitm;
	srand(nSeed);
	for (int j = 0; j < 12; j++) {
		szID[j] = szID[j]+(rand()%12)+17;
	}

	return szID;
}

DWORD WINAPI ClientManager::BeginMonitorWaferClients(LPVOID lParam)
{
#ifdef _WIN32
	gpClientMgr->MonitorWaferClients();
	
	return 0L;
#endif
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::MonitorWaferClients
// Description	    : A very crude monitoring thread routine. This is how it
//                    works. The monitor program is in a "forever while" loop 
//     		          checking the number of items in the collection, retrieving
//			          them one by one, and processes them.
//			          On active clients, this checks to see if the thread has
//			          terminated. If yes, it retrieves the error code and decides
//			          on whether to keep the client in the collection or not.
//			          On inactive clients, the amount of time that the client
//			          has been inactive is calculated and if it is more than the
//			          max. idle time, the client is terminated.
// Return type		: void 
// Argument         : void
// ----------------------------------------------------------------------------
void ClientManager::MonitorWaferClients(void)
{
#ifdef _WIN32
	long			lIndex;
	long			lCount;
	ChildID_t		ulThreadID;

	for(;;) {
		m_pCriticalSection->Lock();
		{
			lIndex = 0;
			lCount = m_ClientCollection.Count();
			
			while (lIndex < lCount) {
				DWORD				dwWaitResult;
				WaferClient*		pWaferClient;
				WaferClientInfo*	pWaferClientInfo;

				pWaferClientInfo = m_ClientCollection[lIndex];
				pWaferClient = pWaferClientInfo->pWaferClient;

				if (pWaferClient->IsActive() == FALSE) {
					time_t tCurrTime;
					time(&tCurrTime);

					if ( (tCurrTime - pWaferClientInfo->lInactiveTime) > m_lTimeout) {
						cout << "Inactivity timer for Client <" << pWaferClient->GetClientID() << ">" << endl;
						
						m_ClientCollection.RemoveAtIndex(lIndex);
						lCount = m_ClientCollection.Count();
						
						pWaferClient->Terminate();

						delete pWaferClient;
						delete pWaferClientInfo;
					}
					else {
						lIndex++;
					}
					continue;
				}

				ulThreadID = pWaferClient->GetChildID();
				dwWaitResult = WaitForSingleObject(ulThreadID, 100);
			
				switch(dwWaitResult) {

				case WAIT_OBJECT_0:
					unsigned long ulTemp;
					BOOL bExit;

					bExit = GetExitCodeThread(ulThreadID, &ulTemp);

					if (bExit == 0) {
						cout << "FATAL Error: GetExitCode Failed -> Reason " << GetLastError() << endl;
					}
					else {
						char* szID = pWaferClient->GetClientID();

						cout << "WaferClient <" << szID << "> completed with exit code " << ulTemp << endl;

						if (ulTemp != 0) {
							m_ClientCollection.RemoveAtIndex(lIndex);

							lCount = m_ClientCollection.Count();
							
							delete pWaferClient;
							delete pWaferClientInfo;
						}
						else {
							pWaferClient->SetInActive();
							pWaferClient->SetChildID(0);

							time(&pWaferClientInfo->lInactiveTime);
							lIndex++;
						}
					}
					break;

				case WAIT_ABANDONED:
					lIndex++;
					cout << "Wait Abandoned.." << endl;
					break;

				case WAIT_TIMEOUT:
					lIndex++;
					cout << "Wait Timedout.." << endl;
					break;

				case WAIT_FAILED:
					lIndex++;
					cout << "Wait Failed... Error Code = " << GetLastError() << endl;
					break;
				}
			}
		}
		m_pCriticalSection->UnLock();
		sleep(5);
	}
#endif
}
