// HobbyIOMonitorDlg.h : header file
//

#if !defined(AFX_HOBBYIOMONITORDLG_H__DFA7D395_E772_44EB_8CF9_C19D4A811543__INCLUDED_)
#define AFX_HOBBYIOMONITORDLG_H__DFA7D395_E772_44EB_8CF9_C19D4A811543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHobbyIOMonitorDlg dialog

class CHobbyIOMonitorDlg : public CDialog
{
// Construction
public:
	void ProcessSendResponse(int nReturnCode, int nBufSize, char *stringOK, char *stringError);
	void LoadConfigurationSettings();
	char m_szDigitalOutPreset0[12];
	char m_szDigitalOutPreset1[12];
	char m_szDigitalOutPreset2[12];
	char m_szDigitalOutPreset3[12];

	void EnableDlgButton(int nButtonID, bool bEnable);
	char m_szAnalogValArray[16][10];
	int m_nAnalogDataCaptureMode;
	bool m_bEnableDigitalWordTrigger;
	void OutputDigitalValue(const char *szPtrString);
	float ConvertRawCountsToVolts(char *ptrStringVal);
	bool m_bShowDigitalAsBinary;
	bool m_bShowAnalogAsRaw;
	bool m_bEnableAnalogScanning;
	bool m_bEnableDigitalScanning;
	bool m_bDigitalInputPending;
	bool m_bScanInProgress;
	void ResetDigitalOutputsToLow(bool AllToLow = true);
	bool m_bSocketIsConnected;
	bool m_bContinousScan;

	bool m_bShowAboutBoxAtStartup;
	bool m_bEchoAppDebugging;

	//CString * ptrCString[15];
	void UpdateDigitalInputPorts();
	void UpdateAnalogInputPorts();
	bool AddToCmdTrailBox(char *szTextString);
	CSocket m_csocketRobot;
	int m_nTimerID;
	int	m_nTimerInterval;

	CHobbyIOMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHobbyIOMonitorDlg)
	enum { IDD = IDD_HOBBYIOMONITOR_DIALOG };
	CButton	m_cntrolRadioDO_HI;
	CString	m_csEditDI0;
	CString	m_csEditDI1;
	CString	m_csEditDI2;
	CString	m_csEditDI3;
	CString	m_csEditDI4;
	CString	m_csEditDI5;
	CString	m_csEditDI6;
	CString	m_csEditDI7;
	CString	m_csEditCmdDisplay;
	CString	m_csEditA0;
	CString	m_csEditA1;
	CString	m_csEditA10;
	CString	m_csEditA11;
	CString	m_csEditA12;
	CString	m_csEditA13;
	CString	m_csEditA14;
	CString	m_csEditA15;
	CString	m_csEditA2;
	CString	m_csEditA3;
	CString	m_csEditA4;
	CString	m_csEditA5;
	CString	m_csEditA6;
	CString	m_csEditA7;
	CString	m_csEditA8;
	CString	m_csEditA9;
	CString	m_csDigitalInputTop;
	CString	m_csDigitalOutputTop;
	UINT	m_nPortNumber;
	CString	m_csPassword;
	CString	m_csEdit_IPAddress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHobbyIOMonitorDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHobbyIOMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg void OnButtonTestbutton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonScan();
	afx_msg void OnButtonConnect();
	afx_msg void OnRadiodo0hi();
	afx_msg void OnRadiodo0lo();
	afx_msg void OnRadiodo1hi();
	afx_msg void OnRadiodo1lo();
	afx_msg void OnRadiodo2hi();
	afx_msg void OnRadiodo2lo();
	afx_msg void OnRadiodo3hi();
	afx_msg void OnRadiodo3lo();
	afx_msg void OnRadiodo4hi();
	afx_msg void OnRadiodo4lo();
	afx_msg void OnRadiodo5hi();
	afx_msg void OnRadiodo5lo();
	afx_msg void OnRadiodo6hi();
	afx_msg void OnRadiodo6lo();
	afx_msg void OnRadiodo7hi();
	afx_msg void OnRadiodo7lo();
	afx_msg void OnButtonOutputDigital();
	afx_msg void OnMenuAnalogDlg();
	afx_msg void OnMenuDigitalPortsDlg();
	afx_msg void OnCheckEnableDigitalScanning();
	afx_msg void OnCheckEnableAnalogScans();
	afx_msg void OnCheckContinuousScan();
	afx_msg void OnRadioAnalogDisplayRaw();
	afx_msg void OnRadioAnalogDisplayVolts();
	afx_msg void OnRadioDisplayBinary();
	afx_msg void OnRadioDisplayText();
	afx_msg void OnCheckEnableDigWordTrigger();
	afx_msg void OnRadio3Floating();
	afx_msg void OnRadio3StickeyLo();
	afx_msg void OnRadio3StickeyHi();
	afx_msg void OnChangeEditInterval();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuConfigure();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnButtonDigitalOutputPreset0();
	afx_msg void OnButtonDigitalOutputPreset1();
	afx_msg void OnButtonDigitalOutputPreset2();
	afx_msg void OnButtonDigitalOutputPreset3();
	afx_msg void OnChangeEditIpaddress();
	afx_msg void OnButtonSendcommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOBBYIOMONITORDLG_H__DFA7D395_E772_44EB_8CF9_C19D4A811543__INCLUDED_)
