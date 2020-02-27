#ifndef MFSOCKINTERFACE_DEFINED
#define MFSOCKINTERFACE_DEFINED

#include "Defs.h"
#include "Portable.h"
#include "TInterface.h"
#include "Idgen.hpp"

#define MAXBUF			20000
#define MAXWAITSECONDS	20
#define MAINFRAME_DCP	"athena9"

struct TCPParams {
	char szServerAddress[MAX_IPADDR_LEN+1];
	short sPort;
};

// ----------------------------------------------------------------------------
// MFSockInterface is the object responsible for communicating to the mainframe.
// The MFSockInterface has to have a PID associated with it for a mainframe 
// session. The PID number is obtained using the IDGenerator.
// ----------------------------------------------------------------------------
class MFSockInterface : public TInterface {
private:
	SOCKET	m_tMFSock;
	char	m_szServerAddress[MAX_IPADDR_LEN+1];
	short	m_sPort;
private:
	int		m_nPID;
	static IDGenerator* m_pPIDGen;
public:
	MFSockInterface();
	virtual ~MFSockInterface();

	virtual BOOL	Initialize(void* pvParams);
	virtual BOOL	ConnectToServer();

	virtual int		Send(char* pSendBuffer, long lLen);
	virtual int		Receive(char* pRecvBuffer, long lLen, int nflags = 0);

	virtual void	Disconnect();
public:
	BOOL	OLDPeek(Int_t nSeconds = 0);
	BOOL	Peek(Int_t nSeconds = 0);
	int		GetPID(void) { return m_nPID; }
};
#endif
