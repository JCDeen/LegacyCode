#ifndef TINTERFACE_DEFINED
#define TINTERFACE_DEFINED

// ----------------------------------------------------------------------------
//
// Generic interface to a transaction server.
//
// ----------------------------------------------------------------------------

class TInterface {
public:
	TInterface() {}
	virtual ~TInterface() {}

	virtual BOOL	Initialize(void* pvParams) = 0;
	virtual BOOL	ConnectToServer() = 0;

	virtual int		Send(char* pSendBuffer, long lLen) = 0;
	virtual int		Receive(char* pRecvBuffer, long lLen, int flags = 0) = 0;

	virtual void	Disconnect() = 0;
};

#endif
