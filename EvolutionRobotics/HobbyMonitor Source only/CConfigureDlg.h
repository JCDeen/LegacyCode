#if !defined(AFX_CONFIGUREDLG_H__8D9F2B29_369E_4EDC_8C9E_D7A606064E14__INCLUDED_)
#define AFX_CONFIGUREDLG_H__8D9F2B29_369E_4EDC_8C9E_D7A606064E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog

class CConfigureDlg : public CDialog
{
// Construction
public:
	CConfigureDlg(CWnd* pParent = NULL);   // standard constructor

	bool m_bSaveAsDefaultIPAddress;
	bool m_bAutoConnect;
	bool m_bShowAboutBoxAtStartup;
	bool m_bEchoAppDebugging;
// Dialog Data
	//{{AFX_DATA(CConfigureDlg)
	enum { IDD = IDD_DIALOG_CONFIGURE };
	CString	m_csEditDOTopConfigure;
	CString	m_csEditDO0c;
	CString	m_csEditDO1c;
	CString	m_csEditDO2c;
	CString	m_csEditDO3c;
	CString	m_csEditDigitalInputTriggerVal;
	CString	m_csDigitalWordNotificationMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRADIODO0HIc();
	afx_msg void OnRADIODO0LOc();
	afx_msg void OnRADIODO1HIc();
	afx_msg void OnRADIODO1LOc();
	afx_msg void OnRADIODO2HIc();
	afx_msg void OnRADIODO2LOc();
	afx_msg void OnRADIODO3HIc();
	afx_msg void OnRADIODO3LOc();
	afx_msg void OnRADIODO4HIc();
	afx_msg void OnRADIODO4LOc();
	afx_msg void OnRADIODO5HIc();
	afx_msg void OnRADIODO5LOc();
	afx_msg void OnRADIODO6HIc();
	afx_msg void OnRADIODO6LOc();
	afx_msg void OnRADIODO7HIc();
	afx_msg void OnRADIODO7LOc();
	afx_msg void OnCheckAutoconnect();
	afx_msg void OnButtonCopydo0();
	afx_msg void OnButtonCopydo1();
	afx_msg void OnButtonCopydo2();
	afx_msg void OnButtonCopydo3();
	afx_msg void OnButtonCopyTriggerval();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckShowaboutbox();
	afx_msg void OnCheckEchoDebugstatements();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREDLG_H__8D9F2B29_369E_4EDC_8C9E_D7A606064E14__INCLUDED_)
