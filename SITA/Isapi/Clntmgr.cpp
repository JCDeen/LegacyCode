#include "ClntMgr.hpp"

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

	TRACE("ClientManager::ClientManager " __FILE__ "\n"); 

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
	TRACE( " ClientManager::~ClientManager\n"); 

	m_pCriticalSection->Lock();
	{
		long nCount = m_ClientCollection.Count();
		WaferClientInfo*	pWaferClientInfo;
		WaferClient*		pWaferClient;

		TRACE("Terminating all WaferClients on shutdown\n");
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
// ALERT!			: Thread stuff removed since this is ISAPI version.
// ----------------------------------------------------------------------------
ClientManager::Initialize()
{
	TRACE( " ClientManager::Initialize\n"); 

	return TRUE; // All thread code in here was deleted JCD
}

// ----------------------------------------------------------------------------
// Function name	: ClientManager::CreateClient
// Description	    : This function creates a reference to an external client
//                    by creating a WaferClient object.
// Return type		: WaferClient* - The actual WaferClient object created.
//                    NULL if failure to create a WaferClient.
// Argument         : WIP_RequestType eRT - The current request type.
// ----------------------------------------------------------------------------
WaferClient* ClientManager::CreateClient(WIP_RequestType eRT)
{
	WaferClient*		pWaferClient = NULL;
	WaferClientInfo*	pWaferClientInfo;

	TRACE( " ClientManager::CreateClient\n"); 

	m_pCriticalSection->Lock();
	{
		pWaferClient = new WaferClient(eRT);
	
		if (pWaferClient->Initialize() == FALSE)
		{
			// Unable to Initialize the WaferClient
			delete pWaferClient;

			pWaferClient = NULL;
		}
		else
		{
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
	TRACE( " ClientManager::SetClientActive\n"); 

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

	TRACE( " ClientManager::VerifyClient\n"); 

	m_pCriticalSection->Lock();
	{
		pWaferClientInfo = m_ClientCollection.Find(szClientID);
	}
	m_pCriticalSection->UnLock();

	if (pWaferClientInfo == NULL)
	{
		TRACE("ClientManager::VerifyClient - ID <%s> not found\n", szClientID);
		return NULL;
	}

	TRACE("ClientManager::VerifyClient - ID <%s> WAS Found\n", szClientID);
	pWaferClient = pWaferClientInfo->pWaferClient;

	// See if the same WaferClient id is already in use...
	if (pWaferClient->IsActive() == TRUE)
	{
		TRACE("ClientManager::VerifyClient - ID <%s> is ALREADY active\n", szClientID);
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

	TRACE( " ClientManager::EndClient\n"); 

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
	TRACE("ClientManager::SetCompleted\n"); 

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
			TRACE("ClientManager::SetCompleted - Fatal Error. Cannot find the ChildID in the Collection\n");
		}
		else {
			pWaferClient->SetInActive();
			pWaferClient->SetChildID(0);

			if (bKeepAlive == FALSE) {
				m_ClientCollection.RemoveAtIndex(i);
				TRACE("ClientManager::SetCompleted - Removed Client <%s> from the collection\n", pWaferClient->GetClientID());
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

	TRACE("ClientManager::GetCount\n"); 

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

	TRACE("ClientManager::GenerateClientID\n"); 

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


void ClientManager::GetClientIDByIndex(int nIndex, char* pszClientID)
{
	TRACE("ClientManager::GetClientIDByIndex\n"); 

	m_pCriticalSection->Lock();
	{
		long nCount = m_ClientCollection.Count();
		WaferClientInfo*	pWaferClientInfo;
		WaferClient*		pWaferClient;

		if(nCount>=0 && nIndex>=0 && nIndex<=nCount)
		{
			pWaferClientInfo = m_ClientCollection[nIndex];
			pWaferClient = pWaferClientInfo->pWaferClient;
			strcpy(pszClientID, pWaferClient->GetClientID());
		}
		else
			strcpy(pszClientID, "NO ID FOUND");
	}
	m_pCriticalSection->UnLock();
}
