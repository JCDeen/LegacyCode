#include <new.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <iostream.h>

#ifndef _WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include "WFSocket.hpp"

#ifndef _WIN32
extern int errno;
#endif


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::WaferSocket
// Description	    : Constructor: Create a WaferSocket.
// Return type		: None
// Argument         : WaferObj* pWfObj - Not much use of this WaferObj as yet.
//                    This object may be used to associate WaferSocket with
//                    another object, like a window or output etc...
//                    eg: pWFObj->Output(). If WaferObj is of type MyDialog/
//                    MyApplicationWindow, then the Output could be redirected
//                    appropriately.
//                    Default value of pWfObj is NULL.
// ----------------------------------------------------------------------------
WaferSocket::WaferSocket(WaferObj* pWfObj)
{
	m_nLastError = 0;
	m_bClosed = FALSE;
	m_pWaferObj = pWfObj;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::WaferSocket
// Description	    : Constructor: Another method for creating a WaferSocket.
//                    This method is typically useful when a server "accepts"
//                    a connection and returns the SOCKET descriptor.
//                    This constructor is also helpful to convert sockets of
//                    other type to WaferSocket.
// Return type		: None
// Argument         : SOCKET s - The descriptor returned by AcceptConnection
// ----------------------------------------------------------------------------
WaferSocket::WaferSocket(SOCKET s)
{
	m_nLastError = 0;
	m_bClosed = FALSE;
	m_pWaferObj = NULL;
	m_Socket = s;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::Create
// Description	    : Create the socket for communication
// Return type		: BOOL - TRUE if the socket could be successfully created.
// Argument         : short Port - Default value = 0;
// ----------------------------------------------------------------------------
BOOL WaferSocket::Create(short Port)
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
	if ( (m_Socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		return FALSE;
	}
	//
	// By setting the SO_LINGER option, we make sure that all data is "flushed"
	// before the socket is closed.
	//
	struct linger stLinger;

	stLinger.l_onoff = 1;
	stLinger.l_linger = 5;

	if (setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char *)&stLinger, sizeof(struct linger)) < 0) {
		GetLastError();
		return FALSE;
	}
	
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::~WaferSocket
// Description	    : Destructor: Checks to see if the socket is already closed
//                    A graceful exit is then initiated.
// Return type		: None
// ----------------------------------------------------------------------------
WaferSocket::~WaferSocket()
{
	// sleep(3); // Remove this once a protocol is established for shutdown.

	if (IsClosed() == FALSE) {
		shutdown(m_Socket, 2); // 0 - Disable receive, 1 - Write, 2 - Both

		closesocket(m_Socket);
		m_bClosed = TRUE;
	}
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::GetLastError
// Description	    : Get the last error that WaferSocket encountered.
// Return type		: int - The error number.
// ----------------------------------------------------------------------------
int WaferSocket::GetLastError()
{
#ifdef _WIN32
	m_nLastError = WSAGetLastError();
#else
	m_nLastError = errno;
#endif
	return m_nLastError;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::GetErrorString
// Description	    : Same as GetLastError, except that it returns a error
//                    string as opposed to error number
// Return type		: char* - The error string.
// ----------------------------------------------------------------------------
char* WaferSocket::GetErrorString()
{
#ifdef _WIN32
	switch (m_nLastError) {
	case WSANOTINITIALISED:
		cout << "A successful WSAStartup must occur before using this function." << endl;
		break;
	case WSAENETDOWN:
		cout << "The Windows Sockets implementation has detected that the network subsystem has failed." << endl;
		break;
	case WSAEAFNOSUPPORT:
		cout << "The specified address family is not supported." << endl;
		break;
	case WSAEINPROGRESS:
		cout << "A blocking Windows Sockets operation is in progress." << endl;
		break;
	case WSAEMFILE:
		cout << "No more file descriptors are available." << endl;
		break;
	case WSAENOBUFS:
		cout << "No buffer space is available. The socket cannot be created." << endl;
		break;
	case WSAEPROTONOSUPPORT:
		cout << "The specified protocol is not supported." << endl;
		break;
	case WSAEPROTOTYPE:
		cout << "The specified protocol is the wrong type for this socket." << endl;
		break;
	case WSAESOCKTNOSUPPORT:
		cout << "The specified socket type is not supported in this address family." << endl;
		break;
	case WSAEFAULT:	
		cout << "The addrlen argument is too small (less than the size of a sockets address structure)" << endl;
		break;
	case WSAEINTR:	
		cout << "The (blocking) call was canceled using WSACancelBlockingCall." << endl;
		break;
	case WSAEINVAL:	
		cout << "The listen function was not invoked before accept." << endl;
		break;
	case WSAENOTSOCK:	
		cout << "The descriptor is not a socket." << endl;
		break;
	case WSAEOPNOTSUPP:	
		cout << "The referenced socket is not a type that supports connection-oriented service." << endl;
		break;
	case WSAEWOULDBLOCK:	
		cout << "The socket is marked as nonblocking and no connections are present to be accepted." << endl;
		break;
	case WSAEADDRINUSE:
		cout << "The specified address is already in use. (See the SO_REUSEADDR socket option under setsockopt)" << endl;
		break;
	case WSAEISCONN:
		cout << "The socket is already connected." << endl;
		break;
	case WSAEALREADY:
	case WSAEDESTADDRREQ:
	case WSAEMSGSIZE:
	case WSAENOPROTOOPT:
	case WSAEPFNOSUPPORT:
	case WSAEADDRNOTAVAIL:
	case WSAENETUNREACH:
	case WSAENETRESET:
	case WSAECONNABORTED:
	case WSAECONNRESET:
	case WSAENOTCONN:
	case WSAESHUTDOWN:
	case WSAETOOMANYREFS:
	case WSAETIMEDOUT:
	case WSAECONNREFUSED:
	case WSAELOOP:
	case WSAENAMETOOLONG:
	case WSAEHOSTDOWN:
	case WSAEHOSTUNREACH:
	case WSAENOTEMPTY:
	case WSAEPROCLIM:
	case WSAEUSERS:
	case WSAEDQUOT:
	case WSAESTALE:
	case WSAEREMOTE:
	case WSAEDISCON:
		cout << "Socket error" << endl;
		break;
	default:
		cout << "Undefined Error Condition " << m_nLastError << endl;
		break;
	}
#endif

	return("Some error String");
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::Send
// Description	    : Sends the data through the socket. The function waits to
//                    send 'all' data before returning from the function.
// Return type		: BOOL - TRUE if data was sent successfully. (This does not
//                    ensure that the packet actually reached the peer entity.
//                    It only implies that the data has been sent from this end)
// Argument         : const char* Message - The packet to be sent.
// Argument         : Int_t nLen - The lenght of the packet.
// ----------------------------------------------------------------------------
BOOL WaferSocket::Send(const char* Message, Int_t nLen)
{
	Int_t	nBytesLeft, nBytesSent = 0;

	nBytesLeft = nLen;
	while(nBytesLeft >  0) {
		nBytesSent = send(m_Socket, Message, nBytesLeft, 0);

		if (nBytesSent <= -1) {
			cout << "Send Failed. Reason " << GetLastError() << endl;
			SetLastError(0);
			return FALSE;
		}

		nBytesLeft -= nBytesSent;
		Message += nBytesSent;
	}
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::Receive
// Description	    : Receive the data. This receive "blocks" till the required
//                    number of bytes is received completely. The nTimeOut flag
//                    is not implemented yet. Part of the code from "Peek()" has
//                    to be embedded within the call to support timeouts.
// Return type		: BOOL 
// Argument         : char* Message
// Argument         : Int_t nBytesToReceive
// ----------------------------------------------------------------------------
BOOL WaferSocket::Receive(char* Message, Int_t nBytesToReceive, Int_t nTimeOut)
{
	Int_t	nReceived;

	while (nBytesToReceive > 0) {
		nReceived = recv(m_Socket, Message, nBytesToReceive, 0);

		if (nReceived == -1) {
			cout << "Receive failed. Reason " << GetLastError() << endl;
			return FALSE;
		}
		else if (nReceived == 0)
			break;

		nBytesToReceive -= nReceived;
		Message += nReceived;
	}
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::Peek
// Description	    : Peek to see if any data is waiting to be read for nSeconds.
//                    Setting the SO_RCVTIMEO option using setsockopt is "NOT"
//                    portable, and it is advised that "select()" be used
//                    instead.
// Return type		: BOOL - TRUE if data is ready to be read. FALSE otherwise.
// Argument         : int nSeconds - Number of seconds to Peek.
// ----------------------------------------------------------------------------
BOOL WaferSocket::Peek(int nSeconds)
{
	struct timeval	stTimeOut;
	int				nCount;
	fd_set			m_fdVar; // Make it member of a WaferSocket after testing.

	stTimeOut.tv_sec = nSeconds;
	stTimeOut.tv_usec = 0;

	FD_ZERO(&m_fdVar);
	FD_SET(m_Socket, &m_fdVar);

	if ((nCount = select(m_Socket+1, &m_fdVar, NULL, NULL, &stTimeOut)) < 0) {
		perror("Peek");
		return FALSE;
	}

	if (nCount == 0) {
		return FALSE;
	}
	else {
		if (FD_ISSET(m_Socket, &m_fdVar) == FALSE) {
			perror("Peek::FD_ISSET");
			return FALSE;
		}
	}
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferSocket::Close
// Description	    : Close the socket gracefully.
// Return type		: void 
// ----------------------------------------------------------------------------
void WaferSocket::Close()
{
	if (IsClosed() == FALSE) {
		shutdown(m_Socket, 2);

		closesocket(m_Socket);
		m_bClosed = TRUE;
	}
}


// ----------------------------------------------------------------------------
// Function name	: WaferClientSocket::WaferClientSocket
// Description	    : Create a client socket. A client socket behaves exactly
//                    like a WaferSocket, but has an extra method to connect
//                    a server.
// Return type		: None
// Argument         : WaferObj* pWfObj
// ----------------------------------------------------------------------------
WaferClientSocket::WaferClientSocket(WaferObj* pWfObj)
	: WaferSocket(pWfObj)
{
}


// ----------------------------------------------------------------------------
// Function name	: WaferClientSocket::ConnectToServer
// Description	    : Given a TCP address and a port number, this connects to
//                    the specified server
// Return type		: TRUE if successful, FALSE otherwise.
// Argument         : char* szAddr - The address of the server
// Argument         : short Port - The port number.
// ----------------------------------------------------------------------------
WaferClientSocket::ConnectToServer(char* szAddr, short Port)
{
	struct sockaddr_in srvAddr;

	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = inet_addr(szAddr);
	srvAddr.sin_port = htons(Port);

	if (connect(m_Socket, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) < 0) {
		return 0;
	}
	return 1;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServerSocket::WaferServerSocket
// Description	    : Constructor
// Return type		: None
// Argument         : WaferObj* pWfObj
// ----------------------------------------------------------------------------
WaferServerSocket::WaferServerSocket(WaferObj* pWfObj)
	: WaferSocket(pWfObj)
{
}


// ----------------------------------------------------------------------------
// Function name	: WaferServerSocket::Create
// Description	    : Create a socket and make it behave like a server.
// Return type		: BOOL 
// Argument         : short Port
// ----------------------------------------------------------------------------
BOOL WaferServerSocket::Create(short Port)
{
	if (WaferSocket::Create(Port) == FALSE)
		return FALSE;
		
	//
	// Uncomment the next line to make it a server that does not block
	// int i = fcntl(m_Socket, F_SETFL, FNDELAY);
	//

	struct sockaddr_in	sa;

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(Port);

	if (bind(m_Socket, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		return FALSE;
	}

	if (listen(m_Socket, 5) < 0) {
		return FALSE;
	}
	return TRUE;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServerSocket::AcceptConnection
// Description	    : Call this method to accept a connection on a server socket.
//                    This method returns just the socket descriptor, which 
//                    should inturn be used to create a WaferSocket, by calling
//                    the constructor of type WaferSocket(SOCKET s);
// Return type		: SOCKET - The socket descriptor associated with the client.
// ----------------------------------------------------------------------------
SOCKET WaferServerSocket::AcceptConnection()
{
	SockAddr clientAddr;
	int nLen = sizeof(clientAddr);

	SOCKET cliSocket = accept(m_Socket, (struct sockaddr *)&clientAddr, &nLen);

	return cliSocket;
}
