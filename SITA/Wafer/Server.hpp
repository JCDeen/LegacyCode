#ifndef _SERVER_HEADER_DEFINED
#define _SERVER_HEADER_DEFINED

#include <signal.h>

#include "Portable.h"
#include "Wip.h"

#include "Clntmgr.hpp"
#include "WFSocket.hpp"
#include "WFChild.hpp"

// ----------------------------------------------------------------------------
// class WaferServer is made as a "Singleton". This means that at any given
// point of time there can be only 'one' instance of Wafer Server.
// ----------------------------------------------------------------------------
class WaferServer {
private:
	// ----------------------------------------------------------------------------
	// Hide the default constructor. An instance of WaferServer could be obtained
	// only by the method WaferServer::Instance
	// ----------------------------------------------------------------------------
	WaferServer();
private:
	// ----------------------------------------------------------------------------
	// The number of child processes/threads running at this time
	// ----------------------------------------------------------------------------
	Int_t				m_nChildProcesses;

	// ----------------------------------------------------------------------------
	// This is the socket that accepts connections from Wafer clients.
	// ----------------------------------------------------------------------------
	WaferServerSocket	m_ServerSocket;

	// ----------------------------------------------------------------------------
	// Instance of a client manager. The server has to Initialize the client manager
	// ----------------------------------------------------------------------------
	ClientManager*		m_pClientManager;

	// ----------------------------------------------------------------------------
	// The instance of the wafer server
	// ----------------------------------------------------------------------------
	static WaferServer*	m_pInstance;
public:
	~WaferServer();

	static WaferServer*	Instance();

	BOOL				ReadINIFile(char* szFileName);
	BOOL				Initialize();

	BOOL				AcceptRequests();
	BOOL				SendErrorResponse(WaferSocket*, long lResponse);
	BOOL				CheckOpenSessions() {return TRUE;}

	BOOL				CreateWaferChild(WaferClient* pWaferClient);
	void				OnChildExit(ChildID_t tChildID, int nExitStatus);

	static DWORD WINAPI	BeginWaferChild(LPVOID lParam);

	static void			SigChildHandler(Int_t iStatus);
	static void			SigIntHandler(Int_t iStatus);
};

#endif //_SERVER_HEADER_DEFINED
