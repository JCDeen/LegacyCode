#include <stdio.h>
#include <iostream.h>
#include <sys/types.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#include "MFInterface.h"
#include "Config.h"


// ----------------------------------------------------------------------------
// Every instance of MFSockInterface requires a PID to be associated with it.
// So, only one instance of a PID generated should exist to find the available
// PIDs. The variable m_pPIDGen of MFSockInterface is hence made static, and as
// with any static variable, it is initialized here..
// ----------------------------------------------------------------------------
IDGenerator* MFSockInterface::m_pPIDGen = NULL;

// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::MFSockInterface
// Description	    : Constructor for Mainframe Socket Interface class.
// Return type		: None
// ----------------------------------------------------------------------------
MFSockInterface::MFSockInterface()
{
	m_tMFSock = 0;
}


// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::~MFSockInterface
// Description	    : Destructor for MFSockInterface. The PID number associated
//                    with this socket is released and the socket connection to
//                    is closed.
// Return type		: None
// ----------------------------------------------------------------------------
MFSockInterface::~MFSockInterface()
{
	if (m_nPID != -1) 
		m_pPIDGen->ReleaseID(m_nPID);
	
	if (m_tMFSock != 0)
		closesocket(m_tMFSock);
}


// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::Initialize
// Description	    : This method needs to be called before any other method on
//                    MFSockInterface is called. This method sets up a socket
//                    for communication to the mainframe and associates a PID
//                    number with the socket.
// Return type		: BOOL - TRUE if initialization is a success, else FALSE.
// Argument         : void* pvParams - This parameter is made a void *, since
//                    the TInterface class had to be made more generic on
//                    Initialize. But the Initialize method of MFSockInterface
//                    expects a parameter of type struct TCPParams.
// ----------------------------------------------------------------------------
BOOL MFSockInterface::Initialize(void* pvParams)
{
#ifdef _WIN32
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 
 
	err = WSAStartup(wVersionRequested, &wsaData); 
 
	if (err != 0) 
		/* Tell the user that we couldn't find a useable */ 
		/* winsock.dll.     */ 
		return FALSE; 
 
	/* Confirm that the Windows Sockets DLL supports 1.1.*/ 
	/* Note that if the DLL supports versions greater */ 
	/* than 1.1 in addition to 1.1, it will still return */ 
	/* 1.1 in wVersion since that is the version we */ 
	/* requested. */ 
 
	if ( LOBYTE( wsaData.wVersion ) != 1 || 
		    HIBYTE( wsaData.wVersion ) != 1 ) { 
		/* Tell the user that we couldn't find a useable */ 
		/* winsock.dll. */ 
		WSACleanup(); 
		return FALSE; 
	}
	/* The Windows Sockets DLL is acceptable. Proceed. */ 
#endif 

	WaferConfig* pstWaferConfig = (WaferConfig *)pvParams;

	strcpy(m_szServerAddress, pstWaferConfig->szTransServerAddress);
	m_sPort = pstWaferConfig->nTransServerPort;

	// ----------------------------------------------------------------------------
	// The IDGenerator has to initialized only once.
	// ----------------------------------------------------------------------------
	if (m_pPIDGen == NULL) {
		m_pPIDGen = new IDGenerator(pstWaferConfig->lBeginPID, pstWaferConfig->lEndPID);
	}

	if ( (m_tMFSock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		return FALSE;
	}

	if ( (m_nPID = m_pPIDGen->GetUniqueID()) == -1) {
		return FALSE;
	}
	cout << "Using PID " << m_nPID << endl;
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::ConnectToServer
// Description	    : This method connects to the server.
// Return type		: BOOL - TRUE if successfully connected, FALSE otherwise.
// ----------------------------------------------------------------------------
BOOL MFSockInterface::ConnectToServer()
{
	struct sockaddr_in	server;
	int nReturnValue;

	server.sin_addr.s_addr = inet_addr(m_szServerAddress);
	server.sin_port = htons((short)m_sPort);  
	server.sin_family = AF_INET;

	cout << "m_szServerAddress:" << m_szServerAddress << endl;
	cout << "m_sPort          :" << m_sPort << endl;

	nReturnValue = connect(m_tMFSock, (struct sockaddr *)&server, sizeof(server));
  
	if (nReturnValue == -1) {
		perror("Connect to Mainframe");
		return FALSE;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::Send
// Description	    : This method is responsible for sending the data to the
//                    mainframe. The data could be split to any number of
//                    packets, since the TCP/IP 'send' does not guarantee
//                    sending all data.
// Return type		: int - The number of bytes sent successfully.
// Argument         : char* pSendBuffer - The buffer to be sent.
// Argument         : long lLen - Length of the buffer
// ----------------------------------------------------------------------------
int MFSockInterface::Send(char* pSendBuffer, long lLen)
{
	long nBytesLeft, nBytesSent = 0;
	long nTotalBytesSent = 0;

	nBytesLeft = lLen;
	while(nBytesLeft >  0) {
		nBytesSent = send(m_tMFSock, (char *)pSendBuffer, nBytesLeft, 0);

		if (nBytesSent <= -1) {
			perror("MFSockInterface::Send");
			return -1;
		}

		nBytesLeft -= nBytesSent;
		pSendBuffer += nBytesSent;
		nTotalBytesSent += nBytesSent;
	}
	return nTotalBytesSent;
}


// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::Receive
// Description	    : This method tries to receive the response for a given
//                    mainframe command.
// Return type		: int - On successful condition returns the number of bytes
//                    received. On error -1.
// Argument         : char* szRecvBuffer - The buffer to be received. Make sure
//                        that memory is allocated to hold the received data.
// Argument         : long lLen - The length of the buffer to receive. But this
//                        is not used at all, since the length for a mainframe
//                        command cannot be guessed.
// Argument         : int nFlags - Any extra flags to be set for the receive
//                        operation.
// ----------------------------------------------------------------------------
int MFSockInterface::Receive(char* szRecvBuffer, long lLen, int nFlags)
{
	int nBytesReceived;

	nBytesReceived = recv(m_tMFSock, szRecvBuffer, MAXBUF, 0);

	if (nBytesReceived == -1) {
		perror("MFSockInterface:Receive");
		return 1;
	}

	return nBytesReceived;
}

// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::Disconnect
// Description	    : This function is called when the inactivity timer on
//                    a mainframe session occurs or when the server is shutdown.
//                    Responses received for logoff is not checked, since this
//                    result cannot be communicated back anyway...
// Return type		: void 
// ----------------------------------------------------------------------------
void MFSockInterface::Disconnect()
{
	char *szMFCmd = "logoff\n";
	char szBuffer[MAXBUF+1];

	Send(szMFCmd, strlen(szMFCmd));

	// If mainframe does not send response in 5 seconds, try again
	if (Peek(5) == FALSE) {
		Send(szMFCmd, strlen(szMFCmd));
	}
	Receive(szBuffer, MAXBUF);
	
	closesocket(m_tMFSock);
	m_tMFSock = 0;
}


// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::OLDPeek
// Description	    : This method checks to see if any response is waiting to
//                    be processed. The method takes the nSeconds argument and
//                    goes into a "nSeconds" loop, checking for data.
// Return type		: BOOL - TRUE if any data is waiting to be received.
//                           FALSE if no data is available.
// Argument         : Int_t nSeconds - Number of seconds to peek for data.
// ----------------------------------------------------------------------------
BOOL MFSockInterface::OLDPeek(Int_t nSeconds)
{
	unsigned long	ulTemp = 1L;

	// Any value for ulTemp other than zero should set the
	// socket in non-blocking mode. (OK???)
	int i = ioctl(m_tMFSock, FIONBIO, &ulTemp);

	BOOL	bResponse = FALSE;
	char	szBuffer[2];

	if (i < 0) {
		perror("Peek:fcntl");
		return FALSE;
	}
	
	int nCount = 0;

	while (nCount <= nSeconds) {
		int nBytes = recv(m_tMFSock, szBuffer, 1, MSG_PEEK);
		if (nBytes > 0) {
			bResponse = TRUE;
			break;
		}
		else {
			if (nBytes == 0) {
				perror("Peek:Recv");
				break;
			}
		}
		nCount++;

		if (nCount <= nSeconds)
			sleep(1);
	 } 
	
	// Reset the socket to blocking state
	ioctl(m_tMFSock, FIONBIO, 0);

	return bResponse;
}

// ----------------------------------------------------------------------------
// Function name	: MFSockInterface::Peek
// Description	    : This method Peeks for 'nSeconds' to see if any data is
//                    ready to be read.
// Return type		: BOOL - TRUE if any data is waiting to be received.
//                           FALSE if no data is available.
// Argument         : Int_t nSeconds - Number of seconds to peek for data.
// ----------------------------------------------------------------------------
BOOL MFSockInterface::Peek(int nSeconds)
{
	struct timeval	stTimeOut;
	int				nCount;
	fd_set			m_fdVar; // Make this as a member variable once Peek is tested.

	stTimeOut.tv_sec = nSeconds;
	stTimeOut.tv_usec = 0;

	FD_ZERO(&m_fdVar);
	FD_SET(m_tMFSock, &m_fdVar);

	if ((nCount = select(m_tMFSock+1, &m_fdVar, NULL, NULL, &stTimeOut)) < 0) {
		perror("Peek");
		return FALSE;
	}

	if (nCount == 0) {
		return FALSE;
	}
	else {
		if (FD_ISSET(m_tMFSock, &m_fdVar) == FALSE) {
			perror("Peek::FD_ISSET");
			return FALSE;
		}
	}
	return TRUE;
}
