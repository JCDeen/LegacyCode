#if !defined(AFX_ISAPISERVER_H__35E4F8E5_0D0D_11D2_AEAD_006008C18682__INCLUDED_)
#define AFX_ISAPISERVER_H__35E4F8E5_0D0D_11D2_AEAD_006008C18682__INCLUDED_

// ISAPISERVER.H - Header file for your Internet Server
//    IsapiServer Extension

#include "resource.h"
#include "ClntMgr.hpp"
#include "WIP.h"
#include "Errors.h"	// Added by ClassView

class CIsapiServerExtension : public CHttpServer
{
private:
	void Login(CHttpServerContext * pCtxt, LPTSTR pstrUserID, LPSTR pstrPassword, LPSTR pstrSystem);
	void Header(CHttpServerContext * pCtxt,  LPTSTR pstrClientID, LPTSTR pstrXValue, LPTSTR pstrYValue);
	void GetPage(CHttpServerContext * pCtxt, LPSTR pstrClientID);
	void Expert(CHttpServerContext *pCtxt, LPTSTR pstrCommand, LPSTR pstrClientID);
	void Logout(CHttpServerContext * pCtxt, LPSTR pstrClientID);

public:
	CIsapiServerExtension();
	~CIsapiServerExtension();

	ClientManager*		m_pClientManager;

	CString m_csHTMLTemplateDir;
	int		m_nTimesRun;
	BOOL	m_bInitOK;
	int		m_nAppListeningPort;
	int		m_nStartPID;
	int		m_nStopPID;
	int		m_nInactivityTimer;
	CString	m_csInactivityTimerUnits;
	CString	m_csDCPIPAddress;
	int		m_nDCPPortNumber;

	CString m_csQuery ;
	CString m_csCommand ;

// Overrides
	// ClassWizard generated virtual function overrides
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_VIRTUAL(CIsapiServerExtension)
	public:
	virtual BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);
	virtual void WriteTitle(CHttpServerContext* pCtxt) const;
	virtual DWORD HttpExtensionProc(EXTENSION_CONTROL_BLOCK *pECB);
	virtual int CallFunction(CHttpServerContext* pCtxt, LPTSTR pszQuery, LPTSTR pszCommand);
	virtual BOOL OnParseError(CHttpServerContext* pCtxt, int nCause);
	//}}AFX_VIRTUAL

	// TODO: Add handlers for your commands here.
	// For example:

	void Default(CHttpServerContext* pCtxt);

	DECLARE_PARSE_MAP()

	//{{AFX_MSG(CIsapiServerExtension)
	//}}AFX_MSG
public:
	//void Test(CHttpServerContext * pCtxt, LPSTR pstrParm);
	void Test(CHttpServerContext * pCtxt, LPSTR E1, LPSTR E2, LPSTR E3, LPSTR E4, LPSTR E5, LPSTR E6, LPSTR E7, LPSTR E8, LPSTR E9, LPSTR E10, LPSTR 
							E11, LPSTR E12, LPSTR E13, LPSTR E14, LPSTR E15, LPSTR E16, LPSTR E17, LPSTR E18, LPSTR E19, LPSTR E20, LPSTR 
							E21, LPSTR E22, LPSTR E23, LPSTR E24, LPSTR E25, LPSTR E26, LPSTR E27, LPSTR E28, LPSTR E29, LPSTR E30, LPSTR E31);

	BOOL m_bDeleteInactiveClients;
	int m_nSecondsToSleep;
	WaferError ProcessRequest(WIP_RequestType eRT, char* pszClientID, void* pvParams);

	void ShowINIValues(CHttpServerContext* pCtxt);
	void Admin(CHttpServerContext * pCtxt, LPSTR pstrVar, LPSTR pstrPassword, LPSTR pstrAdminType);

	BOOL BeginWaferChild(WaferClient* pWaferClient, void* pvParams);

protected:
	static UINT CleanerFunction(LPVOID pvParms);
	int m_nNumberOfClients;
	int	m_nChildProcesses;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISAPISERVER_H__35E4F8E5_0D0D_11D2_AEAD_006008C18682__INCLUDED)
