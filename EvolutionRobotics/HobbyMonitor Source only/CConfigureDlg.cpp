// ConfigureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "CConfigureDlg.h"
#include "ConfigurationSettings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog


CConfigureDlg::CConfigureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDlg)
	m_csEditDOTopConfigure = _T("00000000");
	m_csEditDO0c = _T("00000000");
	m_csEditDO1c = _T("00000000");
	m_csEditDO2c = _T("00000000");
	m_csEditDO3c = _T("00000000");
	m_csEditDigitalInputTriggerVal = _T("00000010"); //_T("00000000");
	m_csDigitalWordNotificationMsg = _T("Digital Word Triggered!\r\nDI6 == LO");
	//}}AFX_DATA_INIT
}


void CConfigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDlg)
	DDX_Text(pDX, IDC_EDIT_DO_TOP_CONFIGURE, m_csEditDOTopConfigure);
	DDX_Text(pDX, IDC_EDIT_DO0c, m_csEditDO0c);
	DDX_Text(pDX, IDC_EDIT_DO1c, m_csEditDO1c);
	DDX_Text(pDX, IDC_EDIT_DO2c, m_csEditDO2c);
	DDX_Text(pDX, IDC_EDIT_DO3c, m_csEditDO3c);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_VAL, m_csEditDigitalInputTriggerVal);
	DDX_Text(pDX, IDC_EDIT_NOTIFYMSG, m_csDigitalWordNotificationMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigureDlg)
	ON_BN_CLICKED(IDC_RADIODO0HIc, OnRADIODO0HIc)
	ON_BN_CLICKED(IDC_RADIODO0LOc, OnRADIODO0LOc)
	ON_BN_CLICKED(IDC_RADIODO1HIc, OnRADIODO1HIc)
	ON_BN_CLICKED(IDC_RADIODO1LOc, OnRADIODO1LOc)
	ON_BN_CLICKED(IDC_RADIODO2HIc, OnRADIODO2HIc)
	ON_BN_CLICKED(IDC_RADIODO2LOc, OnRADIODO2LOc)
	ON_BN_CLICKED(IDC_RADIODO3HIc, OnRADIODO3HIc)
	ON_BN_CLICKED(IDC_RADIODO3LOc, OnRADIODO3LOc)
	ON_BN_CLICKED(IDC_RADIODO4HIc, OnRADIODO4HIc)
	ON_BN_CLICKED(IDC_RADIODO4LOc, OnRADIODO4LOc)
	ON_BN_CLICKED(IDC_RADIODO5HIc, OnRADIODO5HIc)
	ON_BN_CLICKED(IDC_RADIODO5LOc, OnRADIODO5LOc)
	ON_BN_CLICKED(IDC_RADIODO6HIc, OnRADIODO6HIc)
	ON_BN_CLICKED(IDC_RADIODO6LOc, OnRADIODO6LOc)
	ON_BN_CLICKED(IDC_RADIODO7HIc, OnRADIODO7HIc)
	ON_BN_CLICKED(IDC_RADIODO7LOc, OnRADIODO7LOc)
	ON_BN_CLICKED(IDC_CHECK_AUTOCONNECT, OnCheckAutoconnect)
	ON_BN_CLICKED(IDC_BUTTON_COPYDO0, OnButtonCopydo0)
	ON_BN_CLICKED(IDC_BUTTON_COPYDO1, OnButtonCopydo1)
	ON_BN_CLICKED(IDC_BUTTON_COPYDO2, OnButtonCopydo2)
	ON_BN_CLICKED(IDC_BUTTON_COPYDO3, OnButtonCopydo3)
	ON_BN_CLICKED(IDC_BUTTON_COPY_TRIGGERVAL, OnButtonCopyTriggerval)
	ON_BN_CLICKED(IDC_CHECK_SHOWABOUTBOX, OnCheckShowaboutbox)
	ON_BN_CLICKED(IDC_CHECK_ECHO_DEBUGSTATEMENTS, OnCheckEchoDebugstatements)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg message handlers

//#include "ConfigureDlgSettings.h"

void CConfigureDlg::OnOK() 
{
	////////////////////////////////////////////////
	// Get configuration settings from .INI file. //
	////////////////////////////////////////////////
	CConfigurationSettings *pCfg = new CConfigurationSettings;
	pCfg->GetAllSettings();

	////////////////////////////////////////
	// Get the Digital Output Presets ... //
	// & set buttons text = output string.//
	////////////////////////////////////////
	strcpy(pCfg->m_szDigitalOutPreset0, m_csEditDO0c);
	strcpy(pCfg->m_szDigitalOutPreset1, m_csEditDO1c);
	strcpy(pCfg->m_szDigitalOutPreset2, m_csEditDO2c);
	strcpy(pCfg->m_szDigitalOutPreset3, m_csEditDO3c);

	pCfg->m_bShowAboutBoxAtStartup = m_bShowAboutBoxAtStartup;

	pCfg->m_bEchoAppDebugging = m_bEchoAppDebugging;

/********* FINISH fleshing out ....
	m_bEnableDigitalScanning = pCfg->m_bEnableDigitalScanning;
	m_bEnableAnalogScanning = pCfg->m_bEnableAnalogScanning;
	//m_bScanInProgress = pCfg->m_bScanInProgress;

	m_bContinousScan = pCfg->m_bContinousScan;
	m_bShowAnalogAsRaw = pCfg->m_bShowAnalogAsRaw;
	m_bShowDigitalAsBinary = pCfg->m_bShowDigitalAsBinary;

	m_bEnableDigitalWordTrigger = pCfg->m_bEnableDigitalWordTrigger;

	m_nAnalogDataCaptureMode = pCfg->m_nAnalogDataCaptureMode;
	CheckDlgButton(m_nAnalogDataCaptureMode, BST_CHECKED);

	CheckDlgButton(IDC_CHECK1_ENABLE_ANALOG, (m_bEnableAnalogScanning ? BST_CHECKED : BST_UNCHECKED));
	CheckDlgButton(IDC_CHECK_ENABLE_DIGITAL_SCANNING, (m_bEnableDigitalScanning ? BST_CHECKED : BST_UNCHECKED));
	CheckDlgButton(IDC_RADIO_DISPLAY_BINARY, (m_bShowDigitalAsBinary ? BST_CHECKED : BST_UNCHECKED));
	CheckDlgButton(IDC_RADIO_ANALOG_DISPLAY_RAW, (m_bShowAnalogAsRaw ? BST_CHECKED : BST_UNCHECKED));

//	SetDlgItemText(IDC_EDIT_COMMANDENTRY, "play phrase 'This program is GREAT!'");
***************************/
	pCfg->WriteAllSettings();

	delete pCfg;
	
	CDialog::OnOK();
}

void CConfigureDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CConfigureDlg::OnRADIODO0HIc() 
{
	m_csEditDOTopConfigure.SetAt(7,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO0LOc() 
{
	m_csEditDOTopConfigure.SetAt(7,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO1HIc() 
{
	m_csEditDOTopConfigure.SetAt(6,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO1LOc() 
{
	m_csEditDOTopConfigure.SetAt(6,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO2HIc() 
{
	m_csEditDOTopConfigure.SetAt(5,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO2LOc() 
{
	m_csEditDOTopConfigure.SetAt(5,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO3HIc() 
{
	m_csEditDOTopConfigure.SetAt(4,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO3LOc() 
{
	m_csEditDOTopConfigure.SetAt(4,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO4HIc() 
{
	m_csEditDOTopConfigure.SetAt(3,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO4LOc() 
{
	m_csEditDOTopConfigure.SetAt(3,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO5HIc() 
{
	m_csEditDOTopConfigure.SetAt(2,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO5LOc() 
{
	m_csEditDOTopConfigure.SetAt(2,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO6HIc() 
{
	m_csEditDOTopConfigure.SetAt(1,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO6LOc() 
{
	m_csEditDOTopConfigure.SetAt(1,'0');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO7HIc() 
{
	m_csEditDOTopConfigure.SetAt(0,'1');
	UpdateData(FALSE);
}

void CConfigureDlg::OnRADIODO7LOc() 
{
	m_csEditDOTopConfigure.SetAt(0,'0');
	UpdateData(FALSE);
}

//////////////////////////////////////////////////
// Copy DO settings from m_csEditDOTopConfigure //
//////////////////////////////////////////////////

void CConfigureDlg::OnButtonCopydo0() 
{
	m_csEditDO0c = m_csEditDOTopConfigure;
	UpdateData(FALSE);
}

void CConfigureDlg::OnButtonCopydo1() 
{
	m_csEditDO1c = m_csEditDOTopConfigure;
	UpdateData(FALSE);
}

void CConfigureDlg::OnButtonCopydo2() 
{
	m_csEditDO2c = m_csEditDOTopConfigure;
	UpdateData(FALSE);
}

void CConfigureDlg::OnButtonCopydo3() 
{
	m_csEditDO3c = m_csEditDOTopConfigure;
	UpdateData(FALSE);
}

void CConfigureDlg::OnButtonCopyTriggerval() 
{
	m_csEditDigitalInputTriggerVal = m_csEditDOTopConfigure;
	UpdateData(FALSE);
}

/////////////////////////////////////////////

void CConfigureDlg::OnCheckAutoconnect() 
{
	if (IsDlgButtonChecked(IDC_CHECK_AUTOCONNECT))
		m_bAutoConnect = true;
	else
		m_bAutoConnect = false;
}

BOOL CConfigureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_RADIODO0HIc, IDC_RADIODO0LOc, IDC_RADIODO0LOc);   
	CheckRadioButton(IDC_RADIODO1HIc, IDC_RADIODO1LOc, IDC_RADIODO1LOc);   
	CheckRadioButton(IDC_RADIODO2HIc, IDC_RADIODO2LOc, IDC_RADIODO2LOc);   
	CheckRadioButton(IDC_RADIODO3HIc, IDC_RADIODO3LOc, IDC_RADIODO3LOc);   
	CheckRadioButton(IDC_RADIODO4HIc, IDC_RADIODO4LOc, IDC_RADIODO4LOc);   
	CheckRadioButton(IDC_RADIODO5HIc, IDC_RADIODO5LOc, IDC_RADIODO5LOc);   
	CheckRadioButton(IDC_RADIODO6HIc, IDC_RADIODO6LOc, IDC_RADIODO6LOc);   
	CheckRadioButton(IDC_RADIODO7HIc, IDC_RADIODO7LOc, IDC_RADIODO7LOc);

	//////////////////////////////////////////////////////////////
	// Hard coded values for DEMO - change to dymanic later !!! //
	//////////////////////////////////////////////////////////////
	CheckDlgButton(IDC_CHECK_ADD_LINENUMBERS, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_ECHO_DEBUGSTATEMENTS, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_ECHO_TO_CMDTRAIL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_MSGBOX, BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigureDlg::OnCheckShowaboutbox() 
{
	if (IsDlgButtonChecked(IDC_CHECK_SHOWABOUTBOX))
		m_bShowAboutBoxAtStartup = true;
	else
		m_bShowAboutBoxAtStartup = false;
}

void CConfigureDlg::OnCheckEchoDebugstatements() 
{
	if (IsDlgButtonChecked(IDC_CHECK_ECHO_DEBUGSTATEMENTS))
		m_bEchoAppDebugging = true;
	else
		m_bEchoAppDebugging = false;
}
