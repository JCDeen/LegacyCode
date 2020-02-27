#ifndef _CLIENT_MANAGER_INCLUDED
#define _CLIENT_MANAGER_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#include "Portable.h"
#include "Wafermap.h"
#include "WFClient.hpp"
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
	ClientManager(long lTimeout);
	~ClientManager();

	BOOL				Initialize();
	WaferClient*		CreateClient(WaferSocket* pSocket, WIP_RequestType eRT, WIP_ClientType eClientType);
	WaferClient*		VerifyClient(string szClientID);
	void				EndClient(string szClientID);

	void				SetClientActive(WaferClient* pWaferClient, ChildID_t tChildID, BOOL bActive);
	void				SetCompleted(ChildID_t tChildID, BOOL bKeepAlive);

	long				GetCount();
	void				MonitorWaferClients();

	static DWORD WINAPI BeginMonitorWaferClients(LPVOID lParam);
	static char*		GenerateClientID(void);
};

#endif
