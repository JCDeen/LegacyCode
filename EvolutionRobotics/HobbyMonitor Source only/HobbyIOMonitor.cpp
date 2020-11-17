// HobbyIOMonitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "HobbyIOMonitorDlg.h"
#include "ConfigurationSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorApp

BEGIN_MESSAGE_MAP(CHobbyIOMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CHobbyIOMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorApp construction

CHobbyIOMonitorApp::CHobbyIOMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHobbyIOMonitorApp object

CHobbyIOMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorApp initialization

BOOL CHobbyIOMonitorApp::InitInstance()
{
	//SetRegistryKey("CHobbyIOMonitorApp");

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	/////////////////////////////////////////////////
	// Build the .INI file and store all default   // 
	// values there for the first run of this app. //
	/////////////////////////////////////////////////
	CConfigurationSettings *pCfg = new CConfigurationSettings;
	if (pCfg->IsFirstInstance())
	{	pCfg->GetAllSettings();
		pCfg->WriteAllSettings();
	}
	delete pCfg;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif




	CHobbyIOMonitorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
