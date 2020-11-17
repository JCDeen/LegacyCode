// ServerSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "amazingFTP.h"
#include "ServerSetupDlg.h"

// CServerSetupDlg dialog

IMPLEMENT_DYNAMIC(CServerSetupDlg, CDialog)
CServerSetupDlg::CServerSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSetupDlg::IDD, pParent)
{
}

CServerSetupDlg::~CServerSetupDlg()
{
}

void CServerSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOSTNAME, m_serverName); // Change name to m_DlgServerName. etc.
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_userName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Text(pDX, IDC_EDIT_INITIALDIR_REMOTE, m_remoteWorkingDir);
	DDX_Text(pDX, IDC_EDIT_INITIALDIR_LOCAL, m_localWorkingDir);
	DDX_Control(pDX, IDC_COMBO_SETUPNAME, m_ctrlCombo_SetupName);
}


BEGIN_MESSAGE_MAP(CServerSetupDlg, CDialog)
ON_BN_CLICKED(IDC_BUTTON_SAVESETUP, OnBnClickedButtonSavesetup)
ON_BN_CLICKED(IDC_BUTTON_DELETESETUP, OnBnClickedButtonDeletesetup)
ON_BN_CLICKED(IDC_BUTTON_NEWSETUP, OnBnClickedButtonNewsetup)
ON_CBN_SELCHANGE(IDC_COMBO_SETUPNAME, OnCbnSelchangeComboSetupname)
END_MESSAGE_MAP()


// CServerSetupDlg message handlers

BOOL CServerSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	////////////////////////////
	// Populate the combobox. //
	////////////////////////////
	int nLastSetupIndex = (int) AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "LastSetupIndex", 0);
	int nFirstSetupIndex = (int) AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "FirstSetupIndex", 0);
	int nCount = (int) AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "SetupsCount", 0);

	int nSelection = 0;
	int nIndex = nFirstSetupIndex;
	while (nIndex != -99) 
	{
		m_csSetupName.Format("%d.ServerSetupName", nIndex);
		m_csServerName.Format("%d.ServerName", nIndex);
		m_csPassword.Format("%d.Password", nIndex);
		m_csRemoteWorkingDir.Format("%d.RemoteWorkingDir", nIndex);
		m_csLocalWorkingDir.Format("%d.LocalWorkingDir", nIndex);
		m_csUserName.Format("%d.UserName", nIndex);
		m_csNextIndex.Format("%d.NextIndex", nIndex);

		CString csSetupNameValue = AfxGetApp()->GetProfileString("ServerSetup", m_csSetupName.GetBuffer());
		if (csSetupNameValue != "")
		{	
			m_ctrlCombo_SetupName.InsertString(nSelection, csSetupNameValue);
			m_ctrlCombo_SetupName.SetItemData(nSelection, (DWORD) nIndex);

			if (nIndex == nLastSetupIndex)
			{
				m_serverName = AfxGetApp()->GetProfileString("ServerSetup", m_csServerName.GetBuffer(), "--");
				m_password = AfxGetApp()->GetProfileString("ServerSetup", m_csPassword.GetBuffer(), "--");
				m_remoteWorkingDir = AfxGetApp()->GetProfileString("ServerSetup", m_csRemoteWorkingDir.GetBuffer(), "--");
				m_localWorkingDir = AfxGetApp()->GetProfileString("ServerSetup", m_csLocalWorkingDir.GetBuffer(), "--");
				m_userName = AfxGetApp()->GetProfileString("ServerSetup", m_csUserName.GetBuffer(), "--");
				UpdateData(false);
			}
		}

		nIndex = (int) AfxGetApp()->GetProfileInt("ServerSetup", m_csNextIndex.GetBuffer(), 0);
		nSelection++;

		if (nIndex == -99)
			break;
	}

	/////////////////////////////////////
	// Get the last used server setup. //
	/////////////////////////////////////
	m_ctrlCombo_SetupName.SetCurSel(nLastSetupIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CServerSetupDlg::OnBnClickedButtonSavesetup()
{
	int nCurSel = 0;
	int nCurIndex = m_ctrlCombo_SetupName.GetCurSel();
	int nCount = (int) AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "SetupsCount", 0);
	if (nCurIndex == -1)
	{
		nCurSel = nCount;
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "SetupsCount", nCount+1);
	}
	else
		nCurSel = (int) m_ctrlCombo_SetupName.GetItemData(nCurIndex);

	//CString csSetupNameTag;
	//csSetupNameTag.Format("%d.ServerSetup", nCurSel);

	UpdateData(true);

	m_csSetupName.Format("%d.ServerSetupName", nCurSel);
	m_csServerName.Format("%d.ServerName", nCurSel);
	m_csPassword.Format("%d.Password", nCurSel);
	m_csRemoteWorkingDir.Format("%d.RemoteWorkingDir", nCurSel);
	m_csLocalWorkingDir.Format("%d.LocalWorkingDir", nCurSel);
	m_csUserName.Format("%d.UserName", nCurSel);

	m_ctrlCombo_SetupName.SetEditSel(0, -1);
	CString csSetupNameDlg;
	m_ctrlCombo_SetupName.GetWindowText(csSetupNameDlg);

	UpdateData(false);

	AfxGetApp()->WriteProfileString("ServerSetup", m_csSetupName.GetBuffer(), csSetupNameDlg.GetBuffer());
	AfxGetApp()->WriteProfileString("ServerSetup", m_csServerName.GetBuffer(), m_serverName);
	AfxGetApp()->WriteProfileString("ServerSetup", m_csPassword.GetBuffer(), m_password);
	AfxGetApp()->WriteProfileString("ServerSetup", m_csRemoteWorkingDir.GetBuffer(), m_remoteWorkingDir);
	AfxGetApp()->WriteProfileString("ServerSetup", m_csLocalWorkingDir.GetBuffer(), m_localWorkingDir);
	AfxGetApp()->WriteProfileString("ServerSetup", m_csUserName.GetBuffer(), m_userName);

	if (nCurIndex == -1)
	{
		// Set former NextIndex value to the new setup.

		int nLastSavedIndex = (int) AfxGetApp()->GetProfileInt(APP_SETTINGS_STRING, "LastSavedIndex", 0);

		m_csNextIndex.Format("%d.NextIndex", nLastSavedIndex);
		AfxGetApp()->WriteProfileInt("ServerSetup", m_csNextIndex.GetBuffer(), nCurSel);
		AfxGetApp()->WriteProfileInt(APP_SETTINGS_STRING, "LastSavedIndex", nCurSel);

		// Set current NextIndex to flag end of ServerSetup(s).
		m_csNextIndex.Format("%d.NextIndex", nCurSel);
		AfxGetApp()->WriteProfileInt("ServerSetup", m_csNextIndex.GetBuffer(), -99);
	}
}

void CServerSetupDlg::OnBnClickedButtonDeletesetup()
{	
	////////////
	/// Maybe embed 
	if (AfxMessageBox("Are you sure you want to delete this FTP Server Setup ?", MB_YESNO) == IDYES)
	{
		int nCurSel = m_ctrlCombo_SetupName.GetCurSel();
		CString cs;
		cs.Format("Need different scheme\nto name the Setups!\nBecause of Delete %d", nCurSel);
		//m_ctrlCombo_SetupName.DeleteString(nCurSel);
		AfxMessageBox(cs.GetBuffer());
	}
}

void CServerSetupDlg::OnBnClickedButtonNewsetup()
{
	m_serverName.Empty(); 
	m_password.Empty();
	m_remoteWorkingDir.Empty();
	m_localWorkingDir.Empty();
	m_userName.Empty();
	m_ctrlCombo_SetupName.SetEditSel(0, -1);
	m_ctrlCombo_SetupName.Clear(); // CurSel is now = -1.

	UpdateData(false);
}

void CServerSetupDlg::OnCbnSelchangeComboSetupname()
{
	int n = m_ctrlCombo_SetupName.GetCurSel();
	if (n >= 0)
	{
		int nItemData = (int) m_ctrlCombo_SetupName.GetItemData(n);

		m_csServerName.Format("%d.ServerName", nItemData);
		m_csPassword.Format("%d.Password", nItemData);
		m_csRemoteWorkingDir.Format("%d.RemoteWorkingDir", nItemData);
		m_csLocalWorkingDir.Format("%d.LocalWorkingDir", nItemData);
		m_csUserName.Format("%d.UserName", nItemData);
		m_csNextIndex.Format("%d.NextIndex", nItemData);

		CString csSetupNameTag;
		csSetupNameTag.Format("%d.ServerSetup", nItemData);

		m_serverName = AfxGetApp()->GetProfileString("ServerSetup", m_csServerName.GetBuffer(), "--");
		m_password = AfxGetApp()->GetProfileString("ServerSetup", m_csPassword.GetBuffer(), "--");
		m_remoteWorkingDir = AfxGetApp()->GetProfileString("ServerSetup", m_csRemoteWorkingDir.GetBuffer(), "--");
		m_localWorkingDir = AfxGetApp()->GetProfileString("ServerSetup", m_csLocalWorkingDir.GetBuffer(), "--");
		m_userName = AfxGetApp()->GetProfileString("ServerSetup", m_csUserName.GetBuffer(), "--");
		UpdateData(false);
	}
}
