#ifndef _WAFERCLIENT_INCLUDED
#define _WAFERCLIENT_INCLUDED

#include "Portable.h"
#include "WIP.h"

#include "MFInterface.h"

extern struct WaferConfig* gpstWaferConfig;

// ----------------------------------------------------------------------------
// class WaferClient: This is a class to store the state information of an
// external client connected to the Wafer server. Does not have any methods
// other than the Set and Get methods..
// ----------------------------------------------------------------------------
class WaferClient
{
private:
	MFSockInterface*	m_pInterface;
	WIP_RequestType		m_eRequestType;
	ChildID_t			m_tChildID;
	char				m_szClientID[MAX_CLIENTID_LEN+1];
	BOOL				m_bActive;

	void*				m_pvParams;
	int					m_ExitStatus;
	char*				m_szResults;

// Operations
public:
	
	WaferClient(WIP_RequestType eRT, void* pvParams);
	//WaferClient(WaferClient* pWaferClient, void* pvParams);
	~WaferClient();
	void				SetProperties (WIP_RequestType eRT);
	BOOL				Initialize (void);
	WIP_RequestType		GetRequestType (void);
	MFSockInterface*	GetInterface (void);
	char*				GetClientID (void); 
	void				Terminate (void);
	void				SetClientID (char* szID);
	void				SetActive (BOOL bActive);
	void				SetInActive ();
	BOOL				IsActive (void);
	int					Process ();
	void				ProcessLogin ();
	void				ProcessExpert ();
	void				ProcessLogout ();
	void				SetError (int nError);

};

#endif
