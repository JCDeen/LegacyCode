// HobbyIOMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "HobbyIOMonitorDlg.h"
#include "CAnalogPortsDlg.h"
#include "CDigitalPortsDlg.h"
#include "CConfigureDlg.h"
#include "ConfigurationSettings.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLOATING_DATA_CAPTURE IDC_RADIO3_FLOATING
#define LO_DATA_CAPTURE IDC_RADIO3_LOCAP
#define HI_DATA_CAPTURE IDC_RADIO3_HICAP

/**
IDC_RADIO3_FLOATING, OnRadio3Floating)
	ON_BN_CLICKED(IDC_RADIO3_LOCAP, OnRadio3StickeyLo)
	ON_BN_CLICKED(IDC_RADIO3_HICAP
***/
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_csLicense;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	char szLicense[] = //"This application program is copyrighted 2002,\r\n"
					   //"by James Christian Deen.\r\n\r\n"
					   "You are granted the license to use and\r\n"
					   "distribute this application free of charge.\r\n\r\n"
					   "You may not sell it, or any derivitive of\r\n"
					   "it, under any circumstances.\r\n\r\n"
					   "You may not reverse engineer this application\r\n"
					   "for any purpose.";

	//{{AFX_DATA_INIT(CAboutDlg)//
	m_csLicense = _T(szLicense);
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT_LICENSE, m_csLicense);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorDlg dialog

CHobbyIOMonitorDlg::CHobbyIOMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHobbyIOMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHobbyIOMonitorDlg)
	m_csEditDI0 = _T("");
	m_csEditDI1 = _T("");
	m_csEditDI2 = _T("");
	m_csEditDI3 = _T("");
	m_csEditDI4 = _T("");
	m_csEditDI5 = _T("");
	m_csEditDI6 = _T("");
	m_csEditDI7 = _T("");
	m_csEditCmdDisplay = _T("");
	m_csEditA0 = _T("");
	m_csEditA1 = _T("");
	m_csEditA10 = _T("");
	m_csEditA11 = _T("");
	m_csEditA12 = _T("");
	m_csEditA13 = _T("");
	m_csEditA14 = _T("");
	m_csEditA15 = _T("");
	m_csEditA2 = _T("");
	m_csEditA3 = _T("");
	m_csEditA4 = _T("");
	m_csEditA5 = _T("");
	m_csEditA6 = _T("");
	m_csEditA7 = _T("");
	m_csEditA8 = _T("");
	m_csEditA9 = _T("");
	m_csDigitalInputTop = _T("");
	m_csDigitalOutputTop = _T("");
	m_nPortNumber = 9000;
	m_csPassword = _T("Active in Version 1.0");
	m_csEdit_IPAddress = _T("127.0.0.1");
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHobbyIOMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHobbyIOMonitorDlg)
	DDX_Control(pDX, IDC_RADIODO0HI, m_cntrolRadioDO_HI);
	DDX_Text(pDX, IDC_EDIT_DI0, m_csEditDI0);
	DDX_Text(pDX, IDC_EDIT_DI1, m_csEditDI1);
	DDX_Text(pDX, IDC_EDIT_DI2, m_csEditDI2);
	DDX_Text(pDX, IDC_EDIT_DI3, m_csEditDI3);
	DDX_Text(pDX, IDC_EDIT_DI4, m_csEditDI4);
	DDX_Text(pDX, IDC_EDIT_DI5, m_csEditDI5);
	DDX_Text(pDX, IDC_EDIT_DI6, m_csEditDI6);
	DDX_Text(pDX, IDC_EDIT_DI7, m_csEditDI7);
	DDX_Text(pDX, IDC_EDIT_A0, m_csEditA0);
	DDX_Text(pDX, IDC_EDIT_A1, m_csEditA1);
	DDX_Text(pDX, IDC_EDIT_A10, m_csEditA10);
	DDX_Text(pDX, IDC_EDIT_A11, m_csEditA11);
	DDX_Text(pDX, IDC_EDIT_A12, m_csEditA12);
	DDX_Text(pDX, IDC_EDIT_A13, m_csEditA13);
	DDX_Text(pDX, IDC_EDIT_A14, m_csEditA14);
	DDX_Text(pDX, IDC_EDIT_A15, m_csEditA15);
	DDX_Text(pDX, IDC_EDIT_A2, m_csEditA2);
	DDX_Text(pDX, IDC_EDIT_A3, m_csEditA3);
	DDX_Text(pDX, IDC_EDIT_A4, m_csEditA4);
	DDX_Text(pDX, IDC_EDIT_A5, m_csEditA5);
	DDX_Text(pDX, IDC_EDIT_A6, m_csEditA6);
	DDX_Text(pDX, IDC_EDIT_A7, m_csEditA7);
	DDX_Text(pDX, IDC_EDIT_A8, m_csEditA8);
	DDX_Text(pDX, IDC_EDIT_A9, m_csEditA9);
	DDX_Text(pDX, IDC_EDIT_DIGITAL_IN, m_csDigitalInputTop);
	DDX_Text(pDX, IDC_EDIT_DIGITAL_OUT, m_csDigitalOutputTop);
	DDV_MaxChars(pDX, m_csDigitalOutputTop, 8);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPortNumber);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
	DDX_Text(pDX, IDC_EDIT_IPADDRESS, m_csEdit_IPAddress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHobbyIOMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CHobbyIOMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BUTTON_MACROBUTTON1, OnButtonTestbutton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_RADIODO0HI, OnRadiodo0hi)
	ON_BN_CLICKED(IDC_RADIODO0LO, OnRadiodo0lo)
	ON_BN_CLICKED(IDC_RADIODO1HI, OnRadiodo1hi)
	ON_BN_CLICKED(IDC_RADIODO1LO, OnRadiodo1lo)
	ON_BN_CLICKED(IDC_RADIODO2HI, OnRadiodo2hi)
	ON_BN_CLICKED(IDC_RADIODO2LO, OnRadiodo2lo)
	ON_BN_CLICKED(IDC_RADIODO3HI, OnRadiodo3hi)
	ON_BN_CLICKED(IDC_RADIODO3LO, OnRadiodo3lo)
	ON_BN_CLICKED(IDC_RADIODO4HI, OnRadiodo4hi)
	ON_BN_CLICKED(IDC_RADIODO4LO, OnRadiodo4lo)
	ON_BN_CLICKED(IDC_RADIODO5HI, OnRadiodo5hi)
	ON_BN_CLICKED(IDC_RADIODO5LO, OnRadiodo5lo)
	ON_BN_CLICKED(IDC_RADIODO6HI, OnRadiodo6hi)
	ON_BN_CLICKED(IDC_RADIODO6LO, OnRadiodo6lo)
	ON_BN_CLICKED(IDC_RADIODO7HI, OnRadiodo7hi)
	ON_BN_CLICKED(IDC_RADIODO7LO, OnRadiodo7lo)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonOutputDigital)
	ON_COMMAND(ID_MENU_ANALOG_DLG, OnMenuAnalogDlg)
	ON_COMMAND(ID_MENU_DIGITAL_PORTS_DLG, OnMenuDigitalPortsDlg)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DIGITAL_SCANNING, OnCheckEnableDigitalScanning)
	ON_BN_CLICKED(IDC_CHECK1_ENABLE_ANALOG, OnCheckEnableAnalogScans)
	ON_BN_CLICKED(IDC_CHECK_CONTINUOUS_SCAN, OnCheckContinuousScan)
	ON_BN_CLICKED(IDC_RADIO_ANALOG_DISPLAY_RAW, OnRadioAnalogDisplayRaw)
	ON_BN_CLICKED(IDC_RADIO_ANALOG_DISPLAY_VOLTS, OnRadioAnalogDisplayVolts)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_BINARY, OnRadioDisplayBinary)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_TEXT, OnRadioDisplayText)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DIG_WORD_TRIGGER, OnCheckEnableDigWordTrigger)
	ON_BN_CLICKED(IDC_RADIO3_FLOATING, OnRadio3Floating)
	ON_BN_CLICKED(IDC_RADIO3_LOCAP, OnRadio3StickeyLo)
	ON_BN_CLICKED(IDC_RADIO3_HICAP, OnRadio3StickeyHi)
	ON_EN_CHANGE(IDC_EDIT_INTERVAL, OnChangeEditInterval)
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDD_MENU_CONFIGURE, OnMenuConfigure)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_BUTTON_DIGITAL_OUTPUT_PRESET0, OnButtonDigitalOutputPreset0)
	ON_BN_CLICKED(IDC_BUTTON_DIGITAL_OUTPUT_PRESET1, OnButtonDigitalOutputPreset1)
	ON_BN_CLICKED(IDC_BUTTON_DIGITAL_OUTPUT_PRESET2, OnButtonDigitalOutputPreset2)
	ON_BN_CLICKED(IDC_BUTTON_DIGITAL_OUTPUT_PRESET3, OnButtonDigitalOutputPreset3)
	ON_EN_CHANGE(IDC_EDIT_IPADDRESS, OnChangeEditIpaddress)
	ON_BN_CLICKED(IDC_BUTTON_SENDCOMMAND, OnButtonSendcommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorDlg message handlers

BOOL CHobbyIOMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/////////////////////////////////////////////
	// Configure NON-user accessable settings. //
	/////////////////////////////////////////////
	m_bSocketIsConnected = false;
	m_bScanInProgress = false;

	EnableDlgButton(IDC_BUTTON_SCAN, false);
	EnableDlgButton(IDC_BUTTON_DIGITALOUTPUT, false);
	EnableDlgButton(IDC_BUTTON_SENDCOMMAND, false); // Change back to false after DEMO ...

	EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET0, false);
	EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET1, false);
	EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET2, false);
	EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET3, false);

	for (int n=0; n < 16; n++)					// Init analog input edit controls.
		strcpy(m_szAnalogValArray[n], "000000");

	ResetDigitalOutputsToLow();

	/////////////////////////////////////////
	// Configure user accessable settings. //
	/////////////////////////////////////////
	LoadConfigurationSettings();

	m_nTimerInterval = 150;
	SetDlgItemText(IDC_EDIT_INTERVAL, "150");

	SetDlgItemText(IDC_EDIT_COMMANDENTRY, "play phrase 'This program is GREAT!'");

	//////////////////////////////////////
	// This is for debugging only!!!!!! //
	//////////////////////////////////////
	if (m_bEchoAppDebugging)
	{
		char szBuffer[100];
		CEdit *pEdit;
		pEdit = (CEdit*) GetDlgItem(IDC_EDIT_CMD_AUDIT);
		sprintf(szBuffer, "Text Limit = %d", pEdit->GetLimitText());
		AddToCmdTrailBox(szBuffer);
	}

	//Maybe add a message box here instead ?

	if (m_bShowAboutBoxAtStartup)
	{	CAboutDlg dlgAboutLicense;
		dlgAboutLicense.DoModal();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHobbyIOMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHobbyIOMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHobbyIOMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHobbyIOMonitorDlg::OnAbout() 
{
	CAboutDlg myDlg;
	myDlg.DoModal();
}

////////////// ? DEEN ----delete this ????????????????
void CHobbyIOMonitorDlg::OnButtonTestbutton1() 
{
	AddToCmdTrailBox("TEST BUTTON ONE");
}

void CHobbyIOMonitorDlg::OnTimer(UINT nIDEvent) 
{
	if (m_bEnableDigitalScanning)
		UpdateDigitalInputPorts();

	if (m_bEnableAnalogScanning)
		UpdateAnalogInputPorts();

	CDialog::OnTimer(nIDEvent);
}

#define START_SCANNING 0
#define STOP_SCANNING START_SCANNING + 1

void CHobbyIOMonitorDlg::OnButtonScan() 
{	static int nButtonMode = START_SCANNING;

	if (m_bSocketIsConnected)
	{
		if (!m_bContinousScan) // Just scan when button is pressed.
		{
			if (m_bEnableDigitalScanning)
				UpdateDigitalInputPorts();

			if (m_bEnableAnalogScanning)
				UpdateAnalogInputPorts();
		}
		else					// Scan continously.
		{	CButton *pButton;
			pButton = (CButton*) GetDlgItem(IDC_BUTTON_SCAN);

			if (nButtonMode == 0)
			{	
				m_bScanInProgress = true;
				pButton->SetWindowText("Stop Scanning");
				nButtonMode = STOP_SCANNING;

				EnableDlgButton(IDC_CHECK_CONTINUOUS_SCAN, false);
				
				m_nTimerID = SetTimer(1, m_nTimerInterval, 0);
			}
			else
			{	KillTimer(m_nTimerID);
				pButton->SetWindowText("Start Scanning");
				nButtonMode = START_SCANNING;
				m_bScanInProgress = false;

				EnableDlgButton(IDC_CHECK_CONTINUOUS_SCAN, true);
			}
		}
	}
	else
		AddToCmdTrailBox("Socket Not Connected: Can not scan Hobby Ports");

}

#define CONNECT_BUTTON	1
#define DISCONNECT_BUTTON CONNECT_BUTTON + 1

void CHobbyIOMonitorDlg::OnButtonConnect() 
{	static int nButtonState = CONNECT_BUTTON;
	CButton *pButton;
	pButton = (CButton*) GetDlgItem(IDC_BUTTON_CONNECT);

	TRACE("IN TRACE WINDOW\n");

	if (nButtonState == CONNECT_BUTTON)
	{	m_csocketRobot.Create();

/*************
m_nPortNumber);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
	DDX_Text(pDX, IDC_EDIT_IPADDRESS, m_csEdit_IPAddress);
	//}}AFX_DATA_MAP
************/
	
		if (m_csocketRobot.Connect(m_csEdit_IPAddress, m_nPortNumber))
		{
			AddToCmdTrailBox("Connected to robot successfully!");
			m_bSocketIsConnected = true;
			nButtonState = DISCONNECT_BUTTON;
			pButton->SetWindowText("Disconnect Robot");

			EnableDlgButton(IDC_BUTTON_SCAN, true);
			EnableDlgButton(IDC_BUTTON_DIGITALOUTPUT, true);
			EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET0, true);
			EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET1, true);
			EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET2, true);
			EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET3, true);
			EnableDlgButton(IDC_BUTTON_SENDCOMMAND, true);

		}
		else
		{
			AddToCmdTrailBox("Could not connect to ER-1 Robot...");
			AddToCmdTrailBox("Closing created socket.");
			m_csocketRobot.Close();
			m_bSocketIsConnected = false;
		}
	}
	else
	{
		//////////////////////////////////////////////
		// Kill the scanning before closing socket. //
		//////////////////////////////////////////////
		if (m_bScanInProgress)
		{	KillTimer(m_nTimerID); // Disable Timer routine.

			////////////////////////
			// Reset Scan Button. //
			////////////////////////
			pButton->SetWindowText("Start Scanning");
			m_bScanInProgress = false;
		}

		m_csocketRobot.Close(); // Close socket.
	
		AddToCmdTrailBox("Closed socket connection to robot.");

		nButtonState = CONNECT_BUTTON;
		pButton->SetWindowText("Connect Robot");

		EnableDlgButton(IDC_BUTTON_SCAN, false);
		EnableDlgButton(IDC_BUTTON_DIGITALOUTPUT, false);
		EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET0, false);
		EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET1, false);
		EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET2, false);
		EnableDlgButton(IDC_BUTTON_DIGITAL_OUTPUT_PRESET3, false);
		EnableDlgButton(IDC_BUTTON_SENDCOMMAND, false);

		m_bSocketIsConnected = false;
	}
}

bool CHobbyIOMonitorDlg::AddToCmdTrailBox(char *szTextString)
{
	static int nLines = 0;
	CEdit* pStatus;
	pStatus = (CEdit*) GetDlgItem(IDC_EDIT_CMD_AUDIT);

	char szBuffer[150], szTruncated[90];
	memset(szBuffer, '\0', 150);
	memset(szTruncated, '\0', 90);

	strncpy(szTruncated, szTextString, 80);
	sprintf(szBuffer, "%d:%s\r\n", nLines, szTruncated);
	nLines++;

	pStatus->ReplaceSel(szBuffer);
	return TRUE;
}

/**********

  This command displays the current value of one of the 16 analog inputs. (The 16th input
is tied to the battery. This uses the 0-5V scale as well, but it maps to a 0-15.6V scale on
the battery.)
Format:
input analog {<input number> <input number> ...}
Parameter
<input number> This specifies the number of the input to query. See the Analog Pin Out
that follows. The input numbers are the pins labelled A <input number> on the diagram.
You will only have access to inputs 0-14, although pin 15 is shown in the output display.
The resulting output values range from 0 to 65535, where:
  0 represents 0 volts
  65535 represents 5 volts
If the command is given with no arguments, each of the inputs, from 0 to 15, is listed in
the response. For example:
input analog
OK 3328 3328 3328 3392 3328 3328 3328 3264 3328 58432 3328
55040 3328 51648 3328 61568
If the command is issued with arguments, the value of each listed input, from 0 to 15 is
given in the response. For example:
input analog 0 9
OK 3328 58432
input analog 9
OK 58432
  ***********/

//OK 33218 33218 33218 33912 33218 33218 33128 32164 31328 58432 33128 55040 3328 51648 3328 61568NR 


void CHobbyIOMonitorDlg::UpdateAnalogInputPorts(void)
{
	if (!m_bEnableAnalogScanning)
		return;

	char szCmd[25];
	strcpy(szCmd, "input analog\n\r");

	int nErrCode = m_csocketRobot.Send(szCmd, strlen(szCmd));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read this format:                                                                                      //
	// OK 33210 33211 33212 33913 33214 33215 33126 32167 31328 58432 33128 55040 33258 51648 33258 61568\N\R //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (nErrCode != SOCKET_ERROR)
	{	char chBuf[151], *ptrReturn, *ptrNewline;
		int nNumChars = m_csocketRobot.Receive(chBuf, 135);
		chBuf[150] = '\0';

		if ((toupper(chBuf[0]) == 'O' && toupper(chBuf[1]) == 'K') && // Look for "OK".
			(nNumChars > 0 && nNumChars != SOCKET_ERROR))
		{
			ptrReturn = strchr(chBuf, '\r');
			ptrNewline = strchr(chBuf, '\n');
	
			if (ptrReturn != NULL && ptrNewline != NULL)
			{	*ptrReturn = '\0';
				*ptrNewline = '\0';

				//AddToCmdTrailBox(chBuf);

				char *ptrVal[16];
				ptrVal[0] = &chBuf[3];		// Point to 1st digit after "OK"

				/////////////////////////////////////////
				// Parse each analog value into array. //
				/////////////////////////////////////////
				for (int i=1; i<16; i++)
				{	ptrVal[i] = strchr(ptrVal[i-1], ' ')  + 1;
					*(ptrVal[i]-1) = '\0';
				}

				if (m_nAnalogDataCaptureMode == LO_DATA_CAPTURE)
				{	for (int nLo=0; nLo < 16; nLo++)
					{
						if (atoi(ptrVal[nLo]) < atoi(m_szAnalogValArray[nLo]))
							strcpy(m_szAnalogValArray[nLo], ptrVal[nLo]);
						else
							strcpy(ptrVal[nLo], m_szAnalogValArray[nLo]);
					}
				}
				else if (m_nAnalogDataCaptureMode == HI_DATA_CAPTURE)
				{	for (int nHi=0; nHi < 16; nHi++)
					{
						if (atoi(ptrVal[nHi]) > atoi(m_szAnalogValArray[nHi]))
							strcpy(m_szAnalogValArray[nHi], ptrVal[nHi]);
						else
							strcpy(ptrVal[nHi], m_szAnalogValArray[nHi]);
					}
				}

				if (m_bShowAnalogAsRaw)
				{	m_csEditA0.Format("%s", ptrVal[0]);
					m_csEditA1.Format("%s", ptrVal[1]);
					m_csEditA2.Format("%s", ptrVal[2]);
					m_csEditA3.Format("%s", ptrVal[3]);
					m_csEditA4.Format("%s", ptrVal[4]);
					m_csEditA5.Format("%s", ptrVal[5]);
					m_csEditA6.Format("%s", ptrVal[6]);
					m_csEditA7.Format("%s", ptrVal[7]);
					m_csEditA8.Format("%s", ptrVal[8]);
					m_csEditA9.Format("%s", ptrVal[9]);
					m_csEditA10.Format("%s", ptrVal[10]);
					m_csEditA11.Format("%s", ptrVal[11]);
					m_csEditA12.Format("%s", ptrVal[12]);
					m_csEditA13.Format("%s", ptrVal[13]);
					m_csEditA14.Format("%s", ptrVal[14]);
					m_csEditA15.Format("%s", ptrVal[15]);
				}
				else
				{	m_csEditA0.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[0]));
					m_csEditA1.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[1]));
					m_csEditA2.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[2]));
					m_csEditA3.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[3]));
					m_csEditA4.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[4]));
					m_csEditA5.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[5]));
					m_csEditA6.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[6]));
					m_csEditA7.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[7]));
					m_csEditA8.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[8]));
					m_csEditA9.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[9]));
					m_csEditA10.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[10]));
					m_csEditA11.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[11]));
					m_csEditA12.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[12]));
					m_csEditA13.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[13]));
					m_csEditA14.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[14]));
					m_csEditA15.Format("%1.8f", ConvertRawCountsToVolts(ptrVal[15]));
				}

				UpdateData(false);
			}
			TRACE("Input Analog sent OK");
		}
		else
		{	
			TRACE("ERROR Receiving \"Input Analog\" Cmd");

			ProcessSendResponse(nNumChars, 150, "Cmd Sent OK", "ERR Receiving Input Analog Cmd");

			m_csEditA0 = "INPUT";
			m_csEditA1 = "ERROR";
		}
	}
	else
	{	
		TRACE("ERROR Sending \"Input Analog\" Cmd");
		m_csEditA0 = "Send";
		m_csEditA1 = "ERROR";
	}
}

/***************
input digital
If the robot is properly connected, the response is:
OK <hex number>
Example:
OK ff
Each input port is represented as one bit of the result code, where digital input (DI) 0 is
the least significant bit and DI 7 is the most significant bit, so ff indicates that all the
inputs are high. OK fe would mean that all but input 0 was high.
Output Digital
This command sets the digital outputs (DO) to the 8 bit value represented by
<hex number>. Note that digital output (DO) 0 is the least significant bit and DO 7 is
*************/

void CHobbyIOMonitorDlg::UpdateDigitalInputPorts()
{
	if (!m_bEnableDigitalScanning)
		return;

	m_bDigitalInputPending = true;

	char szCmd[] = "input digital\n\r";
	int nErrCode = m_csocketRobot.Send(szCmd, strlen(szCmd));

	if (nErrCode != SOCKET_ERROR)
	{
		char chBuf[50];

		int nNumChars = m_csocketRobot.Receive(chBuf, 10);
		chBuf[5]= '\0';
		chBuf[3] = toupper(chBuf[3]);
		chBuf[4] = toupper(chBuf[4]);

		if ((toupper(chBuf[0]) == 'O' && toupper(chBuf[1]) == 'K') && // Look for 'OK'
			(nNumChars > 0 && nNumChars != SOCKET_ERROR))
		{
			char szCmdTrailBuf[100];
			sprintf(szCmdTrailBuf, "Digital In: %s", chBuf);
			AddToCmdTrailBox(szCmdTrailBuf);
	
			m_csDigitalInputTop.Format("%s", &chBuf[3]); // Echo "raw" reading in top Edit box.

			int intValueHi, intValueLo = 0;

			//////////////////////////////////////////////////
			// Parse & decode 1st nybble.                   //
			// Convert ascii value of char to numeric value.//
			//////////////////////////////////////////////////
			if (isdigit(chBuf[3]))
				intValueLo = chBuf[3] - 48;	// Digits 0 - 9
			else
				intValueLo = chBuf[3] - 55; // Chars A - F
	
			//////////////////////////////////////////////////
			// Parse & decode 2nd nybble.                   //
			// Convert ascii value of char to numeric value.//
			//////////////////////////////////////////////////
			if (isdigit(chBuf[4]))
				intValueHi = chBuf[4] - 48; // Digits 0 - 9
			else
				intValueHi = chBuf[4] - 55; // Chars A - F


			char szBinaryValLo[12], szBinaryValHi[12], szBinaryVal[12];

			_itoa(intValueLo, szBinaryValLo, 2);
			_itoa(intValueHi, szBinaryValHi, 2);

			memset(szBinaryVal, '\0', 12);
			memset(szBinaryVal, '0', 4-strlen(szBinaryValLo)); // Add leading zeros.
			strncat(szBinaryVal, szBinaryValLo, 4);			   // Append 1st nybble.

			memset(&szBinaryVal[4], '0', 4-strlen(szBinaryValHi)); // Add leading zeros.
			strncat(szBinaryVal, szBinaryValHi, 4);				   // Append 2nd nybble.

			/////////////////////////////////////////////////////
			// Test if Digital Input Trigger condition is met. //
			/////////////////////////////////////////////////////
			if (m_bEnableDigitalWordTrigger) // 10111110 test value
			{
				if (szBinaryVal[1] == '0') // Hard Coded for DI6 == '0' for now ...
				{	
					KillTimer(m_nTimerID);
	
					AddToCmdTrailBox("Digital Word Triggered !");
					if (AfxMessageBox("Digital Word Triggered!\nDI6 == LO\nRe-trigger ?", MB_YESNO) == IDNO)
						m_bEnableDigitalWordTrigger = false;

					if (m_bScanInProgress)
						m_nTimerID = SetTimer(1, m_nTimerInterval, 0);
				}
			}

			if (m_bShowDigitalAsBinary)
			{	m_csEditDI0.Format("%c", szBinaryVal[7]);
				m_csEditDI1.Format("%c", szBinaryVal[6]);
				m_csEditDI2.Format("%c", szBinaryVal[5]);
				m_csEditDI3.Format("%c", szBinaryVal[4]);
				m_csEditDI4.Format("%c", szBinaryVal[3]);
				m_csEditDI5.Format("%c", szBinaryVal[2]);
				m_csEditDI6.Format("%c", szBinaryVal[1]);
				m_csEditDI7.Format("%c", szBinaryVal[0]);
			}
			else
			{	m_csEditDI0.Format("%s", szBinaryVal[7] == '0' ? "LO" : "HI");
				m_csEditDI1.Format("%s", szBinaryVal[6] == '0' ? "LO" : "HI");
				m_csEditDI2.Format("%s", szBinaryVal[5] == '0' ? "LO" : "HI");
				m_csEditDI3.Format("%s", szBinaryVal[4] == '0' ? "LO" : "HI");
				m_csEditDI4.Format("%s", szBinaryVal[3] == '0' ? "LO" : "HI");
				m_csEditDI5.Format("%s", szBinaryVal[2] == '0' ? "LO" : "HI");
				m_csEditDI6.Format("%s", szBinaryVal[1] == '0' ? "LO" : "HI");
				m_csEditDI7.Format("%s", szBinaryVal[0] == '0' ? "LO" : "HI");
			}
		}
		else
		{	m_csDigitalInputTop = "Digital Input";
			if (m_bEchoAppDebugging)
				AddToCmdTrailBox("Receiving \"Input Digital\" Cmd");
		}
		UpdateData(FALSE);
	}
	else
		AddToCmdTrailBox("Sending \"Input Digital\" Cmd");
	
	m_bDigitalInputPending = false;
}

void CHobbyIOMonitorDlg::OnRadiodo0hi() 
{
	m_csDigitalOutputTop.SetAt(7,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo0lo() 
{
	m_csDigitalOutputTop.SetAt(7,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo1hi() 
{
	m_csDigitalOutputTop.SetAt(6,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo1lo() 
{
	m_csDigitalOutputTop.SetAt(6,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo2hi() 
{
	m_csDigitalOutputTop.SetAt(5,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo2lo() 
{
	m_csDigitalOutputTop.SetAt(5,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo3hi() 
{
	m_csDigitalOutputTop.SetAt(4,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo3lo() 
{
	m_csDigitalOutputTop.SetAt(4,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo4hi() 
{
	m_csDigitalOutputTop.SetAt(3,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo4lo() 
{
	m_csDigitalOutputTop.SetAt(3,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo5hi() 
{
	m_csDigitalOutputTop.SetAt(2,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo5lo() 
{
	m_csDigitalOutputTop.SetAt(2,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo6hi() 
{
	m_csDigitalOutputTop.SetAt(1,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo6lo() 
{
	m_csDigitalOutputTop.SetAt(1,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo7hi() 
{
	m_csDigitalOutputTop.SetAt(0,'1');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::OnRadiodo7lo() 
{
	m_csDigitalOutputTop.SetAt(0,'0');
	UpdateData(FALSE);
}

void CHobbyIOMonitorDlg::ResetDigitalOutputsToLow(bool AllToLow)
{
	if (AllToLow == false)
	{	CheckRadioButton(IDC_RADIODO0HI, IDC_RADIODO0LO, IDC_RADIODO0HI);   
		CheckRadioButton(IDC_RADIODO1HI, IDC_RADIODO1LO, IDC_RADIODO1HI);   
		CheckRadioButton(IDC_RADIODO2HI, IDC_RADIODO2LO, IDC_RADIODO2HI);   
		CheckRadioButton(IDC_RADIODO3HI, IDC_RADIODO3LO, IDC_RADIODO3HI);   
		CheckRadioButton(IDC_RADIODO4HI, IDC_RADIODO4LO, IDC_RADIODO4HI);   
		CheckRadioButton(IDC_RADIODO5HI, IDC_RADIODO5LO, IDC_RADIODO5HI);   
		CheckRadioButton(IDC_RADIODO6HI, IDC_RADIODO6LO, IDC_RADIODO6HI);   
		CheckRadioButton(IDC_RADIODO7HI, IDC_RADIODO7LO, IDC_RADIODO7HI);

		m_csDigitalOutputTop = "11111111";
		UpdateData(FALSE);
	}
	else
	{	CheckRadioButton(IDC_RADIODO0HI, IDC_RADIODO0LO, IDC_RADIODO0LO);   
		CheckRadioButton(IDC_RADIODO1HI, IDC_RADIODO1LO, IDC_RADIODO1LO);   
		CheckRadioButton(IDC_RADIODO2HI, IDC_RADIODO2LO, IDC_RADIODO2LO);   
		CheckRadioButton(IDC_RADIODO3HI, IDC_RADIODO3LO, IDC_RADIODO3LO);   
		CheckRadioButton(IDC_RADIODO4HI, IDC_RADIODO4LO, IDC_RADIODO4LO);   
		CheckRadioButton(IDC_RADIODO5HI, IDC_RADIODO5LO, IDC_RADIODO5LO);   
		CheckRadioButton(IDC_RADIODO6HI, IDC_RADIODO6LO, IDC_RADIODO6LO);   
		CheckRadioButton(IDC_RADIODO7HI, IDC_RADIODO7LO, IDC_RADIODO7LO);

		m_csDigitalOutputTop = "00000000";
		UpdateData(FALSE);
	}
}

#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08

#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

void CHobbyIOMonitorDlg::OutputDigitalValue(const char *szPtrString)
{
	KillTimer(m_nTimerID);

	char szBuffer[9];
	strncpy(szBuffer, szPtrString, 8);
	szBuffer[8] = '\0';

	int nDigitalOutLO = 0, nDigitalOutHI = 0;
	
	nDigitalOutHI = (szBuffer[0] == '1') ? nDigitalOutHI | BIT_3 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[1] == '1') ? nDigitalOutHI | BIT_2 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[2] == '1') ? nDigitalOutHI | BIT_1 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[3] == '1') ? nDigitalOutHI | BIT_0 : nDigitalOutHI;

	nDigitalOutLO = (szBuffer[4] == '1') ? nDigitalOutLO | BIT_3 : nDigitalOutLO;;
	nDigitalOutLO = (szBuffer[5] == '1') ? nDigitalOutLO | BIT_2 : nDigitalOutLO;
	nDigitalOutLO = (szBuffer[6] == '1') ? nDigitalOutLO | BIT_1 : nDigitalOutLO;
	nDigitalOutLO = (szBuffer[7] == '1') ? nDigitalOutLO | BIT_0 : nDigitalOutLO;

	char szDigitalOutHexValHI[2], szDigitalOutHexValLO[2];
	szDigitalOutHexValHI[1] = '\0';
	szDigitalOutHexValLO[1] = '\0';

	_itoa(nDigitalOutLO, szDigitalOutHexValLO, 16);
	_itoa(nDigitalOutHI, szDigitalOutHexValHI, 16);

	char szCmd[50];
	sprintf(szCmd, "output digital %c%c\n\r",toupper(szDigitalOutHexValHI[0]), toupper(szDigitalOutHexValLO[0]));

	int nErrCode = m_csocketRobot.Send(szCmd, strlen(szCmd));

	if (m_bEchoAppDebugging)
	{
		if (nErrCode == SOCKET_ERROR)
			AddToCmdTrailBox("SOCKET_ERROR");
		else
		{	char chResponse[201];
			int nRet = m_csocketRobot.Receive(chResponse, 150);
			ProcessSendResponse(nRet, 150, "Dig Out Sent OK", "ERR Receiving Dig Out response");
		}
	}

	AddToCmdTrailBox(szCmd);

	if (m_bScanInProgress)
		m_nTimerID = SetTimer(1, m_nTimerInterval, 0);

}

void CHobbyIOMonitorDlg::OnButtonOutputDigital() 
{
	OutputDigitalValue((LPCTSTR) m_csDigitalOutputTop);
}

void CHobbyIOMonitorDlg::OnMenuAnalogDlg() 
{
	CAnalogPortsDlg dlgAnalog;
	dlgAnalog.DoModal();	
}

void CHobbyIOMonitorDlg::OnMenuDigitalPortsDlg() 
{
	CDigitalPortsDlg dlgDigital;
	dlgDigital.DoModal();	
}

void CHobbyIOMonitorDlg::OnCheckEnableDigitalScanning() 
{
	if (IsDlgButtonChecked(IDC_CHECK_ENABLE_DIGITAL_SCANNING))
	{	m_bEnableDigitalScanning = true;
		TRACE("Digital Scanning: ON");
	}
	else
	{	m_bEnableDigitalScanning = false;
		TRACE("Digital Scanning: OFF");
	}
}

void CHobbyIOMonitorDlg::OnCheckEnableAnalogScans() 
{
	if (IsDlgButtonChecked(IDC_CHECK1_ENABLE_ANALOG))
	{	m_bEnableAnalogScanning = true;
		TRACE("Analog Scanning: ON");
	}
	else
	{	m_bEnableAnalogScanning = false;
		TRACE("Analog Scanning: OFF");
	}
}

void CHobbyIOMonitorDlg::OnCheckContinuousScan() 
{
	if (IsDlgButtonChecked(IDC_CHECK_CONTINUOUS_SCAN))
	{	
		SetDlgItemText(IDC_BUTTON_SCAN, "Scan Continously");
		m_bContinousScan = true;
	}
	else
	{	
		SetDlgItemText(IDC_BUTTON_SCAN, "Scan Once");
		m_bContinousScan = false;
	}
}

void CHobbyIOMonitorDlg::OnRadioAnalogDisplayRaw() 
{
	if (IsDlgButtonChecked(IDC_RADIO_ANALOG_DISPLAY_RAW))
	{	
		m_bShowAnalogAsRaw = true;
		TRACE("Display Analog as Raw");
	}
}

void CHobbyIOMonitorDlg::OnRadioAnalogDisplayVolts() 
{
	if (!IsDlgButtonChecked(IDC_RADIO_ANALOG_DISPLAY_RAW))
	{	
		m_bShowAnalogAsRaw = false;
		TRACE("Display Analog as Volts");
	}
}

void CHobbyIOMonitorDlg::OnRadioDisplayBinary() 
{
	if (IsDlgButtonChecked(IDC_RADIO_DISPLAY_BINARY))
	{	
		m_bShowDigitalAsBinary = true;
		TRACE("Show DIGITAL as BINARY");
	}
}

void CHobbyIOMonitorDlg::OnRadioDisplayText() 
{
	if (IsDlgButtonChecked(IDC_RADIO_DISPLAY_TEXT))
	{	
		m_bShowDigitalAsBinary = false;
		TRACE("Show DIGITAL as TEXT");
	}
}

///////////////////////////////////
// 0 counts maps to 0 volts.     //
// 65535 counts maps to 5 volts. //
///////////////////////////////////

float CHobbyIOMonitorDlg::ConvertRawCountsToVolts(char *ptrStringVal)
{
	// float cast is to cancel compiler warning.
	float fReturnVal = (float) ((atof(ptrStringVal) * 5.0 ) / 65535.0);
	return fReturnVal;
}

void CHobbyIOMonitorDlg::OnCheckEnableDigWordTrigger() 
{
	if (IsDlgButtonChecked(IDC_CHECK_ENABLE_DIG_WORD_TRIGGER))
	{	
		m_bEnableDigitalWordTrigger = true;
		TRACE("Enable Digital Word Trigger");
	}
	else
	{	
		m_bEnableDigitalWordTrigger = false;
		TRACE("Disabled Digital Word Trigger");
	}
}

void CHobbyIOMonitorDlg::OnRadio3Floating() 
{
	m_nAnalogDataCaptureMode = FLOATING_DATA_CAPTURE;
	TRACE("Analog Data Capture: Floating");	
}

void CHobbyIOMonitorDlg::OnRadio3StickeyLo() 
{
	m_nAnalogDataCaptureMode = LO_DATA_CAPTURE;	
	TRACE("Analog Data Capture: Stickey Lo");	

	for (int n=0; n < 16; n++)
		strcpy(m_szAnalogValArray[n], "99999");
}

void CHobbyIOMonitorDlg::OnRadio3StickeyHi() 
{
	m_nAnalogDataCaptureMode = HI_DATA_CAPTURE;	
	TRACE("Analog Data Capture: Stickey Hi");

	for (int n=0; n < 16; n++)
		strcpy(m_szAnalogValArray[n], "00000");
}

void CHobbyIOMonitorDlg::OnChangeEditInterval() 
{
	m_nTimerInterval = GetDlgItemInt(IDC_EDIT_INTERVAL, NULL, false) ;

	char szReflect[100];
	sprintf(szReflect, "Interval = %d", m_nTimerInterval);
	TRACE(szReflect);

}

void CHobbyIOMonitorDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	AddToCmdTrailBox("RIGHT MOUSE");
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CHobbyIOMonitorDlg::OnMenuConfigure() 
{
	CConfigureDlg myDlg;
	if (myDlg.DoModal() == IDOK)
	{	LoadConfigurationSettings();
		//UpdateData(true);
	}
}

void CHobbyIOMonitorDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	AddToCmdTrailBox("What's This Help Goes Here ...");
}

void CHobbyIOMonitorDlg::OnButtonDigitalOutputPreset0() 
{
	OutputDigitalValue(m_szDigitalOutPreset0);
}

void CHobbyIOMonitorDlg::OnButtonDigitalOutputPreset1() 
{
	OutputDigitalValue(m_szDigitalOutPreset1);
}

void CHobbyIOMonitorDlg::OnButtonDigitalOutputPreset2() 
{
	OutputDigitalValue(m_szDigitalOutPreset2);
}

void CHobbyIOMonitorDlg::OnButtonDigitalOutputPreset3() 
{
	OutputDigitalValue(m_szDigitalOutPreset3);
}

///////////////////////////////////////////////////////////////////////
// Use to keep from duplicating the same old button enabling code... //
///////////////////////////////////////////////////////////////////////
void CHobbyIOMonitorDlg::EnableDlgButton(int nButtonID, bool bEnable)
{
	CButton *pButton;
	pButton = (CButton*) GetDlgItem(nButtonID);
	pButton->EnableWindow(bEnable);
}


////////////////////////
// Add a Load Settings Function Here:

/***
------ Configuration Dialog --------
Digital Input Word Trigger:
EchoToCmdTrail 1
MessageBox 0
TriggerRepeatedly 0
TriggerValue 11001100
NotificationMessage "Digital Input Word Triggered"

Digital Output Presets:
DO0 Value 00000000
DO1 Value 00001111
DO2 Value 11110000
DO3 Value 11111111

Cmd Trail Settings:
AddLineNumbers 1
EchoDebugging 0
SaveToFileOnExit 0

Misc Settings:
Label AI15 as Battery 1
Auto Connect at Program Start 0

-------- Main Dialog ---------
Analog Inputs:
Readings Mode 0,1,2
Display Format 0,1
Enable Analog Scanning 1

Digital Inputs:
Display Format 0,1
Enable Digital Word Trigger 0
Enable Digital Scanning 1

Scan Settings:
Continuous Scan 0
Scan Interval 150

Socket Connection:
IP Address 127.0.0.1
Port Number 9000
  ****/

void CHobbyIOMonitorDlg::LoadConfigurationSettings()
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
	strcpy(m_szDigitalOutPreset0, pCfg->m_szDigitalOutPreset0);
	strcpy(m_szDigitalOutPreset1, pCfg->m_szDigitalOutPreset1);
	strcpy(m_szDigitalOutPreset2, pCfg->m_szDigitalOutPreset2);
	strcpy(m_szDigitalOutPreset3, pCfg->m_szDigitalOutPreset3);

	SetDlgItemText(IDC_BUTTON_DIGITAL_OUTPUT_PRESET0, m_szDigitalOutPreset0);
	SetDlgItemText(IDC_BUTTON_DIGITAL_OUTPUT_PRESET1, m_szDigitalOutPreset1);
	SetDlgItemText(IDC_BUTTON_DIGITAL_OUTPUT_PRESET2, m_szDigitalOutPreset2);
	SetDlgItemText(IDC_BUTTON_DIGITAL_OUTPUT_PRESET3, m_szDigitalOutPreset3);

	m_bShowAboutBoxAtStartup = pCfg->m_bShowAboutBoxAtStartup;

	m_bEchoAppDebugging = pCfg->m_bEchoAppDebugging;

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

	UpdateData(false);

//	SetDlgItemText(IDC_EDIT_COMMANDENTRY, "play phrase 'This program is GREAT!'");

	delete pCfg;
}

void CHobbyIOMonitorDlg::OnChangeEditIpaddress() 
{
	UpdateData(true);
}

void CHobbyIOMonitorDlg::OnButtonSendcommand() 
{
	char szCmd[155], szBuffer[151];

	GetDlgItemText(IDC_EDIT_COMMANDENTRY, szBuffer, 150);
	szBuffer[150] = '\0';

	sprintf(szCmd, "%s\n\r", szBuffer);

	int nErrCode = m_csocketRobot.Send(szCmd, strlen(szCmd));
	if (nErrCode == SOCKET_ERROR)
		AddToCmdTrailBox("SOCKET_ERROR");
	else
	{	char chResponse[201];
		int nRet = m_csocketRobot.Receive(chResponse, 150);
		ProcessSendResponse(nRet, 150, "Cmd Sent OK", "ERR Receiving Send Cmd response");
	}
}

void CHobbyIOMonitorDlg::ProcessSendResponse(int nReturnCode, int nBufSize, char *stringOK, char *stringError)
{
	if (nReturnCode == SOCKET_ERROR)
		TRACE("ProcessSendResponse: nReturnCode == SOCKET_ERROR");

	if (nReturnCode > 0 && nReturnCode < nBufSize && nReturnCode != SOCKET_ERROR)
		AddToCmdTrailBox(stringOK);
	else
		AddToCmdTrailBox(stringError);
}
