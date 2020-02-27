#ifndef WAFERSOCKET_INCLUDED
#define WAFERSOCKET_INCLUDED

//------------------------------------------------------------------------------
//
// File WFSocket.hpp.
//
// This is the header file that defines the socket interfaces available for
// communication between the UI or CGI program, to the Wafer Server
//
// The file is best viewed with tab space set to "4".
//------------------------------------------------------------------------------

#include "Waferobj.hpp"
#include "Portable.h"

typedef struct sockaddr_in SockAddr;

class WaferSocket {
private:
	int			m_nLastError;
	BOOL		m_bClosed;
protected:
	SOCKET		m_Socket;
	WaferObj*	m_pWaferObj;
public:
	WaferSocket() { m_bClosed = FALSE;}
	WaferSocket(WaferObj* pWfObj);
	WaferSocket(SOCKET s);
	~WaferSocket();

	virtual BOOL 	Create(short Port);

	BOOL			Send(const char* Message, Int_t len);
	BOOL			Receive(char* Message, Int_t len, Int_t nTimeOut=0);

	BOOL			Peek(int nSeconds);

	Int_t			GetLastError();
	void			SetLastError(int nError) { m_nLastError = nError; }
	char*			GetErrorString();
	void			Close();

	BOOL			IsClosed() { return m_bClosed; }
};

class WaferClientSocket : public WaferSocket {
public:
	WaferClientSocket(WaferObj* pWfObj = NULL);
	
	virtual BOOL	Create(short Port=0) {return WaferSocket::Create(Port);}
	int				ConnectToServer(char* szAddr, short Port);
};

class WaferServerSocket : public WaferSocket {
public:
	WaferServerSocket(WaferObj* pWfObj = NULL);

	virtual BOOL	Create(short Port);
	SOCKET			AcceptConnection();
};
#endif
