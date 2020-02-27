#if !defined(AFX_WAFERCHILD_H__B403F461_F54D_11D1_AEAD_006008C18682__INCLUDED_)
#define AFX_WAFERCHILD_H__B403F461_F54D_11D1_AEAD_006008C18682__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WaferChild.h : header file
//

#include <string>

#include "WIP.h"
#include "WFClient.hpp"

#pragma warning(disable:4786)

using namespace std ;
using namespace std::rel_ops ;

//#define WM_WAFERCHILDMSG	WM_USER+1000

/////////////////////////////////////////////////////////////////////////////
// CWaferChild thread

class CWaferChild {
protected:
	CWaferChild();          // protected constructor used by dynamic creation

protected:
	void*				m_pvParams;
	MFSockInterface*	m_pInterface;
	WIP_RequestType		m_eRequestType;
	int					m_ExitStatus;
	string				m_szClientID;
	char*				m_szResults;

// Operations
public:
	CWaferChild(WaferClient* pWaferClient, void* pvParams); // Sudhakar
	
	int  Process();
	void ProcessLogin();
	void ProcessExpert();
	void ProcessLogout();

	void Begin() {}
	void Exit(void);

	void SetError(int nError);

protected:
	virtual ~CWaferChild();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAFERCHILD_H__B403F461_F54D_11D1_AEAD_006008C18682__INCLUDED_)
