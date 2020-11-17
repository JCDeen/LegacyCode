// mfcClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "amazingFTP.h"
#include "RemoteDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmazingFTP

BEGIN_MESSAGE_MAP(CAmazingFTP, CWinApp)
	//{{AFX_MSG_MAP(CAmazingFTP)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmazingFTP construction

CAmazingFTP::CAmazingFTP()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAmazingFTP object

CAmazingFTP theApp;

/////////////////////////////////////////////////////////////////////////////
// CAmazingFTP initialization

BOOL CAmazingFTP::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//SetRegistryKey("AmazingRobotics"); // Causes all .INI settings to go to/from registry.

	/////////////////////////////////////////////////////////////////////
	// Move this stuff to some kind of Installation Routine / program. //
	/////////////////////////////////////////////////////////////////////
	if (AfxGetApp()->GetProfileString(APP_SETTINGS_STRING, "LocalPath", NULL) == "")
	{
		AfxGetApp()->WriteProfileString(APP_SETTINGS_STRING, "LocalPath", "C:\\");
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "GetAllDrives", 1);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "EncryptPasswords", 1);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "OnStartShowSetupDlg", 1);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "OnStartShowSplashScreen", 1);
		AfxGetApp()->WriteProfileString(APP_SETTINGS_STRING, "OnStartConnect", "--NONE--");
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "SetupsCount", 0);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "FirstSetupIndex", 0);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "LastSelectedIndex", 1);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "LastSavedIndex", 1);
	}

	if (AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "SetupsCount", 0) == 0)
	{
		AfxGetApp()->WriteProfileString("ServerSetup", "0.ServerSetupName", "Microsoft --SAMPLE--");
		AfxGetApp()->WriteProfileString("ServerSetup", "0.ServerName", "ftp.microsoft.com");
		AfxGetApp()->WriteProfileString("ServerSetup", "0.Password", "guest@unknown");
		AfxGetApp()->WriteProfileString("ServerSetup", "0.RemoteWorkingDir", "/");
		AfxGetApp()->WriteProfileString("ServerSetup", "0.LocalWorkingDir", "C:\\");
		AfxGetApp()->WriteProfileString("ServerSetup", "0.UserName", "anonymous");
		AfxGetApp()->WriteProfileInt("ServerSetup", "0.NextIndex", 1);

		AfxGetApp()->WriteProfileString("ServerSetup", "1.ServerSetupName", "NASA KSC --SAMPLE--");
		AfxGetApp()->WriteProfileString("ServerSetup", "1.ServerName", "ftp.ksc.nasa.gov");
		AfxGetApp()->WriteProfileString("ServerSetup", "1.Password", "guest@unknown");
		AfxGetApp()->WriteProfileString("ServerSetup", "1.RemoteWorkingDir", "/pub/winvn/");
		AfxGetApp()->WriteProfileString("ServerSetup", "1.LocalWorkingDir", "C:\\");
		AfxGetApp()->WriteProfileString("ServerSetup", "1.UserName", "anonymous");
		AfxGetApp()->WriteProfileInt("ServerSetup", "1.NextIndex", -99);

		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "SetupsCount", 2);
	}

	CRemoteDlg dlg;
	m_pMainWnd = &dlg;
	m_pRemoteDlg = &dlg;		// Redundant. Streamline later! JCD
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CAmazingFTP::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

