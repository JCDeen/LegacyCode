#ifndef _CLIENT_MANAGER_INCLUDED
#define _CLIENT_MANAGER_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#include "Portable.h"
#include "WaferMap.h"
#include "WaferClient.h"
#include "Critical.h"

struct WaferClientInfo {
	WaferClient*	pWaferClient;
	time_t			lInactiveTime;
};

typedef WaferMap<string, WaferClientInfo*> ClientCollection_t;

// ----------------------------------------------------------------------------
//
// ClientManager: This class is responsible for creating and maintaining
// "WaferClient" objects in a collection. A "WaferClient" object is an internal
// representation for an external client that connects to the Wafer server and
// requests a service.
//
// ----------------------------------------------------------------------------

class ClientManager {
private:
	long					m_lTimeout; // The inactive Timeout for each Wafer client
	ClientCollection_t		m_ClientCollection; // Collection of Wafer clients.
	WFCriticalSection*		m_pCriticalSection; // Critical section class
public:
	void				GetClientIDByIndex(int nIndex, char* pszClientID);
	ClientManager(long lTimeout);
	~ClientManager();

	BOOL				Initialize();
	WaferClient*		CreateClient(WIP_RequestType eRT);
	WaferClient*		VerifyClient(string szClientID);
	void				EndClient(string szClientID);

	void				SetClientActive(WaferClient* pWaferClient, ChildID_t tChildID, BOOL bActive);
	void				SetCompleted(ChildID_t tChildID, BOOL bKeepAlive);

	long				GetCount();

	static char*		GenerateClientID(void);
};

#endif
