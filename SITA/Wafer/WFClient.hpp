#ifndef _WAFERCLIENT_INCLUDED
#define _WAFERCLIENT_INCLUDED

#include "Portable.h"
#include "Wip.h"

#include "WFSocket.hpp"
#include "MFInterface.h"

extern struct WaferConfig* gpstWaferConfig;

// ----------------------------------------------------------------------------
// class WaferClient: This is a class to store the state information of an
// external client connected to the Wafer server. Does not have any methods
// other than the Set and Get methods..
// ----------------------------------------------------------------------------
class WaferClient {
private:
	MFSockInterface*	m_pInterface;
	WaferSocket*		m_pWaferSocket;
	WIP_ClientType		m_eClientType;
	WIP_RequestType		m_eRequestType;
	ChildID_t			m_tChildID;
	char				m_szClientID[MAX_CLIENTID_LEN+1];
	BOOL				m_bActive;
public:

	WaferClient(WaferSocket* pSocket, WIP_RequestType eRT, WIP_ClientType eClientType)
	{
		m_pInterface = new MFSockInterface;
		m_pWaferSocket = pSocket;
		m_eRequestType = eRT;
		m_eClientType = eClientType;
	}

	~WaferClient()
	{
		delete m_pInterface;
	}

	void SetProperties(WaferSocket* pSocket, WIP_RequestType eRT)
	{
		m_pWaferSocket = pSocket;
		m_eRequestType = eRT;
	}

	BOOL Initialize(void)
	{
		return m_pInterface->Initialize(gpstWaferConfig);
	}

	ChildID_t			GetChildID(void) { return m_tChildID; }
	void				SetChildID(ChildID_t tID) { m_tChildID = tID; }

	WIP_RequestType		GetRequestType(void) { return m_eRequestType; }
	WIP_ClientType		GetClientType(void) { return m_eClientType; }
	MFSockInterface*	GetTransaction(void) { return m_pInterface; }
	WaferSocket*		GetWaferSocket(void) { return m_pWaferSocket; }
	char*				GetClientID(void) { return m_szClientID; }

	void Terminate(void)
	{
		m_pInterface->Disconnect();
	}

	void SetClientID(char* szID)
	{
		strcpy(m_szClientID, szID);
	}

	void SetActive(BOOL bActive = TRUE)
	{
		m_bActive = bActive;
	}
	
	void SetInActive()
	{
		m_bActive = FALSE;
		delete m_pWaferSocket;
	}
	
	BOOL IsActive(void) {return (m_bActive);}
};

#endif
