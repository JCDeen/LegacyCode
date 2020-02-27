#include "Server.hpp"
#include "Config.h"

// ----------------------------------------------------------------------------
// Configuration information of the Wafer server is stored in this structure.
// This has been made global since some of the other modules require the 
// configuration information as well. Alternately, the configuration files
// could have been split on a module basis. This would have made us maintain
// more than one configuration file.
//
// If having a global variable seems too bad, you can make this as a class with
// all members static!!
// ----------------------------------------------------------------------------
WaferConfig* gpstWaferConfig;

// ----------------------------------------------------------------------------
// Initialize the static variable WaferServer::m_pInstance
// ----------------------------------------------------------------------------
WaferServer* WaferServer::m_pInstance = NULL;


// ----------------------------------------------------------------------------
// Function name	: main
// Description	    : This does the following:
//                    1. Creates and instance of the Wafer Server
//                    2. Reads the INI file for configuration information (same dir as .exe)
//                    3. Initialize the server
//                    4. Get the server ready to accept requests from external
//                       clients.
// Return type		: int - The exit code of the program.
// Argument         : int argc - The count of the arguments.
// Argument         : char **argv - The input parameters to the program. This
//                    not being utilized properly at this stage. We may provide
//                    options to specify the configuration information or the
//                    name of the configuration file (This is hardcoded to
//                    "WFServer.INI" for the time being.
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	WaferServer* pWaferServer = WaferServer::Instance();

	gpstWaferConfig = new WaferConfig;

	if (pWaferServer->ReadINIFile(CONFIG_FILE_NAME) == FALSE) {
		cout << "Fatal Error reading from the INI File " << CONFIG_FILE_NAME << endl;
		exit(1);
	}

	if (pWaferServer->Initialize() == FALSE) {
		perror("WaferServer Server Initialize");
		exit(1);
	}
	cout << "Initialized the Server" << endl;

	pWaferServer->AcceptRequests();
	
	return 0;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::WaferServer
// Description	    : Tha constructor of the Wafer server. Set the appropriate
//                    signal handler routines to be invoked.
//                    SIGCHLD - Signal indicating the exit of a child process.
//                    SIGINT - The program is interrupted.. Like Ctrl-C.
// Return type		: None
// ----------------------------------------------------------------------------
WaferServer::WaferServer()
{
#ifndef _WIN32
	if (signal(SIGCHLD, SIG_IGN) != SIG_IGN) {
		signal(SIGCHLD, WaferServer::SigChildHandler);
	}
#endif
	if (signal(SIGINT, SIG_IGN) != SIG_IGN) {
		signal(SIGINT, WaferServer::SigIntHandler);
	}

	m_nChildProcesses = 0;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::Instance
// Description	    : Return an instance of the Wafer Server.
// Return type		: WaferServer* 
// ----------------------------------------------------------------------------
WaferServer* WaferServer::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new WaferServer;
	}
	return m_pInstance;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::~WaferServer
// Description	    : Destructor of the wafer server. Currently the program
//                    exits completely on this.
// Return type		: None.
// ----------------------------------------------------------------------------
WaferServer::~WaferServer()
{
	if (m_nChildProcesses != 0) {
		cout << "Send signal to child process on server's termination" << endl;
	}

	delete m_pClientManager;
	m_ServerSocket.Close();

	exit(0);
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::Initialize
// Description	    : Initialize the WaferServer. This includes:
//                    1. Starting up the client manager
//                    2. Create a socket for accepting client requests.
//                    3. Initialize the client manager.
// Return type		: BOOL 
// ----------------------------------------------------------------------------
BOOL WaferServer::Initialize()
{
	m_pClientManager = new ClientManager(gpstWaferConfig->lTimeout*60);

	if (m_ServerSocket.Create(gpstWaferConfig->nListenPort) == FALSE) {
		return FALSE;
	}

	if (m_pClientManager->Initialize() == FALSE) {
		return FALSE;
	}

	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::ReadINIFile
// Description	    : Read the INI file and get the required values. The format
//                    of the INI file is as follows:
//                    Line 1: The port to listen to.
//                    Line 2: Starting PID to use.
//                    Line 3: The end of PID number.
//                    Line 4: The Inactivity timeout value.
//                    Line 5: The IP address of Mainframe DCP/Transaction Server
//                    Line 6: The port number of Mainframe DCP/Transaction Server
// Return type		: BOOL 
// Argument         : char* szFileName
// ----------------------------------------------------------------------------
BOOL WaferServer::ReadINIFile(char* szFileName)
{
	//
	// The ifstream used below is the one from the standard template
	// library. Seems to work just fine, even without the ios | nocreate
	// option.
	//
	// ifstream ServerFile(szFileName, ios::in | ios::nocreate);
	//
	ifstream ServerFile(szFileName, ios::in);

	if (!ServerFile) {
		return FALSE;
	}

	ServerFile >> gpstWaferConfig->nListenPort;
	ServerFile >> gpstWaferConfig->lBeginPID;
	ServerFile >> gpstWaferConfig->lEndPID;
	ServerFile >> gpstWaferConfig->lTimeout;
	ServerFile >> gpstWaferConfig->szTransServerAddress;
	ServerFile >> gpstWaferConfig->nTransServerPort;

	ServerFile.close();

	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::AcceptRequests
// Description	    : This is the routine that the MainThread/ParentProcess of
//                    Wafer server loops into. It does not do anything when the
//                    are no requests coming into the server.
// Return type		: BOOL 
// ----------------------------------------------------------------------------
BOOL WaferServer::AcceptRequests()
{
	for (;;) {
		SOCKET	s;

		cout << "Ready to Accept Connections from clients.." << endl;
		if ( (s = m_ServerSocket.AcceptConnection()) != -1) {
			WaferSocket*		pClientSocket;
			WaferClient*		pWaferClient;
			WIP_RequestHeader	stWIPReqHdr;

			pClientSocket = new WaferSocket(s);

			if (pClientSocket->Receive((char *)&stWIPReqHdr, sizeof(WIP_RequestHeader)) == FALSE) {
				pClientSocket->Close();

				delete pClientSocket;
				perror("Receive Signon Request");
				continue;
			}

			stWIPReqHdr.eRequestType = (WIP_RequestType)ntohs(stWIPReqHdr.eRequestType);
			stWIPReqHdr.eClientType = (WIP_ClientType)ntohs(stWIPReqHdr.eClientType);
			
			if (stWIPReqHdr.eRequestType == LOGIN_REQUEST) {
				pWaferClient = m_pClientManager->CreateClient(pClientSocket, stWIPReqHdr.eRequestType, stWIPReqHdr.eClientType);

				if (pWaferClient == NULL) {
					SendErrorResponse(pClientSocket, SESSION_UNAVAILABLE);
					delete pClientSocket;
					continue;
				}
				else {
				}
			}
			else {
				pWaferClient = m_pClientManager->VerifyClient(stWIPReqHdr.szClientID);

				if (pWaferClient == NULL) {
					SendErrorResponse(pClientSocket, INVALID_SESSION_ID);

					delete pClientSocket;
					continue;
				}
				else {
					pWaferClient->SetProperties(pClientSocket, stWIPReqHdr.eRequestType);
				}
			}
			if (CreateWaferChild(pWaferClient) == FALSE) {
				m_pClientManager->EndClient(pWaferClient->GetClientID());
				delete pClientSocket;
			}
		}
	}
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::CreateWaferChild
// Description	    : This function is responsible for starting up a WaferChild
//                    When used in a windows environment, "CreateThread" is
//                    called.
//                    In a Unix environment, "fork" is called.
// Return type		: BOOL - TRUE if successful on starting a WaferChild,
//                           FALSE otherwise.
// Argument         : WaferClient* pWaferClient - WaferClient is the object used
//                    by WaferChild for processing client requests.
// ----------------------------------------------------------------------------
BOOL WaferServer::CreateWaferChild(WaferClient* pWaferClient)
{
	ChildID_t	tChildID;

#ifdef _WIN32
	DWORD dwThreadID;
	
	if ((tChildID = CreateThread (NULL, 0, WaferServer::BeginWaferChild, pWaferClient, 0, &dwThreadID)) == NULL) {
		cout << "Create Thread failed. Reason -> " << GetLastError() << endl;
		WaferSocket* pSocket = pWaferClient->GetWaferSocket();
		SendErrorResponse(pSocket, FORK_FAILED);

		pSocket->Close();
		return FALSE;
	}
#else
	if ( (tChildID = fork()) == 0 ) {
		BeginWaferChild(pWaferClient);
	}
	else {
		if (tChildID < 0) {
			WaferSocket* pSocket = pWaferClient->GetWaferSocket();
			SendErrorResponse(pSocket, FORK_FAILED);

			pSocket->Close();
			return FALSE;
		}
	}
#endif
	m_pClientManager->SetClientActive(pWaferClient, tChildID, TRUE);
	m_nChildProcesses++;

	cout << "Process/Thread ID of WaferServer Child <" << tChildID << ">" << endl;

	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::BeginWaferChild
// Description	    : The new thread/process starts with this function. This is
//                    like the "main" for that thread/process.
// Return type		: DWORD WINAPI 
// Argument         : LPVOID lParam - This is the "WaferClient" object. Had to
//                    defined as LPVOID, since "CreateThread" expects this
//                    function to have a parameter of type LPVOID.
// ----------------------------------------------------------------------------
DWORD WINAPI WaferServer::BeginWaferChild(LPVOID lParam)
{
	WaferClient* pWaferClient = (WaferClient*)lParam;

	WaferChild* pBC = new WaferChild(pWaferClient);

	pBC->Begin();
	pBC->Exit();

	delete pBC;

	return 0L;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::SendErrorResponse
// Description	    : Sends any error response that occurred while processing
//                    the ClientID.
// Return type		: BOOL True: Successfully sent the error response.
//                         False: Sending the error response failed.
// Argument         : WaferSocket* pSocket: The socket associated with the client
// Argument         : long lResponse: Error response number.
// ----------------------------------------------------------------------------
BOOL WaferServer::SendErrorResponse(WaferSocket* pSocket, long lResponse)
{
	cout << "Sending back error response..." << endl;

	WIP_ResponseHeader	stWIPResponse;

	stWIPResponse.eResponseType = (WIP_ResponseType)htons((short)WIP_ERROR);
	if (pSocket->Send((char *)&stWIPResponse, sizeof(WIP_ResponseHeader)) == FALSE) {
		return FALSE;
	}
	
	ErrorResponseData	stErrorResponse;

	switch(lResponse) {
	case SESSION_UNAVAILABLE:
		strcpy(stErrorResponse.szMessage, "Unable to find a connection for Transaction");
		break;
	case INVALID_SESSION_ID:
		strcpy(stErrorResponse.szMessage, "Session timed out or invalid ID");
		break;
	case FORK_FAILED:
		strcpy(stErrorResponse.szMessage, "Wafer Server is low on system resources");
		break;
	};
	
	cout << "Reason for error " << stErrorResponse.szMessage << endl;
	if (pSocket->Send((char *)&stErrorResponse, sizeof(ErrorResponseData)) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Function name	: WaferServer::OnChildExit
// Description	    : This function is called whenever the WaferServer Child dies.
// Return type		: void 
// Argument         : ChildID_t tChildID - The process or the threadID of the
//                    Wafer child. (obtained in BeginWaferChild)
// Argument         : int nExitStatus - This is the exit code of the Child
//                    process or the worked thread.
// ----------------------------------------------------------------------------
void WaferServer::OnChildExit(ChildID_t tChildID, int nExitStatus)
{	
	// -------------------------------------------------------------
	// Need to process the nExitStatus more carefully.
	// For the time being, exit status of zero means that connection
	// should be active.
	// -------------------------------------------------------------
	if (nExitStatus == 0) {
		m_pClientManager->SetCompleted(tChildID, TRUE);
	}
	else {
		m_pClientManager->SetCompleted(tChildID, FALSE);
	}
	m_nChildProcesses--;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::SigIntHandler
// Description	    : 
// Return type		: void 
// Argument         : int i
// ----------------------------------------------------------------------------
void WaferServer::SigIntHandler(int i)
{
	delete WaferServer::Instance();
}

// ----------------------------------------------------------------------------
// Function name	: WaferServer::SigChildHandler
// Description	    : Signal handler routine to handle the signal SIGCHLD (Unix
//                    implementation only)
// Return type		: void 
// Argument         : int i - This contains the signal number and the exit code
// ----------------------------------------------------------------------------
void WaferServer::SigChildHandler(int i)
{
#ifndef _WIN32
	int			nExitStatus;
	int			status;
	ChildID_t	tChildProcessID;

	//
	// Set the signal handler immediately to SigChildHandler. In Unix, the default
	// behaviour for SIGCHLD will be exhibited till this signal is redirected to
	// the signal handler routine.
	//
	signal(SIGCHLD, SigChildHandler);

	tChildProcessID = wait(&status);
	nExitStatus = WEXITSTATUS(status);

	cout << "WaferChild <" << tChildProcessID << "> completed with exit code " << nExitStatus << endl;

	WaferServer::Instance()->OnChildExit(tChildProcessID, nExitStatus);
#endif
}
