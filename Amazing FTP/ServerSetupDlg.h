#pragma once
#include "afxwin.h"


// CServerSetupDlg dialog

class CServerSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerSetupDlg)

public:
	CServerSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerSetupDlg();
	BOOL CServerSetupDlg::OnInitDialog(); 

// Dialog Data
	enum { IDD = IDD_DIALOG_SERVERSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_serverName;
	CString m_userName;
	CString m_password;
	CString m_remoteWorkingDir;
	CString m_localWorkingDir;

	CString m_csSetupName;
	CString m_csServerName;
	CString m_csPassword;
	CString m_csRemoteWorkingDir;
	CString m_csLocalWorkingDir;
	CString m_csUserName;
	CString m_csNextIndex;

	CComboBox m_ctrlCombo_SetupName;

	afx_msg void OnBnClickedButtonSavesetup();
	afx_msg void OnBnClickedButtonDeletesetup();
	BOOL m_ctrlCheck_UseOnNextOpen;
	afx_msg void OnBnClickedButtonNewsetup();
	afx_msg void OnCbnSelchangeComboSetupname();
};
