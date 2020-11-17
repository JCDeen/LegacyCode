// HobbyIOMonitor.h : main header file for the HOBBYIOMONITOR application
//

#if !defined(AFX_HOBBYIOMONITOR_H__A1B54695_F3F8_4D8A_B594_490BF13363A1__INCLUDED_)
#define AFX_HOBBYIOMONITOR_H__A1B54695_F3F8_4D8A_B594_490BF13363A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorApp:
// See HobbyIOMonitor.cpp for the implementation of this class
//

class CHobbyIOMonitorApp : public CWinApp
{
public:
	CHobbyIOMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHobbyIOMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHobbyIOMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOBBYIOMONITOR_H__A1B54695_F3F8_4D8A_B594_490BF13363A1__INCLUDED_)
