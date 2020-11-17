// CRemoteDlg.cpp : implementation file
//
#include "stdafx.h"
#include "amazingFTP.h"
#include "RemoteDlg.h"
#include "ftp.h"
#include "UserInputDlg.h"
#include "ServerSetupDlg.h"

//#include <stdlib.h>
//#include <stdio.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
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
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog
CRemoteDlg::CRemoteDlg(CWnd* pParent /*=NULL*/) : CDialog(CRemoteDlg::IDD, pParent)
, m_ctrlCheckKeepAlive(FALSE)
, m_bBackupLocalFiles(FALSE)
, m_bBackupRemoteFiles(FALSE)
{
	m_pPath = NULL;
	m_nKeepAlivePeriodSeconds = 60; // Default value. Override in setups.
	//{{AFX_DATA_INIT(CRemoteDlg)
	//}}AFX_DATA_INIT
}

int CRemoteDlg::nCounter = 21;

void CRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDlg)
	DDX_Control(pDX, IDC_LIST_REMOTE, m_RemoteListing);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_CMDS_TRAIL, m_editCtrlCmdsTrail);
	DDX_Control(pDX, IDC_STATIC_REMOTEDIR, m_ctrlStaticRemoteDir);
	DDX_Control(pDX, IDC_LIST_LOCALDIR, m_ctrlLocalListing);
	DDX_Control(pDX, IDC_STATIC_LOCALDIR, m_ctrlStaticLocalDir);
	DDX_Check(pDX, IDC_CHECK_KEEPALIVE, m_ctrlCheckKeepAlive);
	DDX_Check(pDX, IDC_CHECK_BACKUP_LOCAL_FILES, m_bBackupLocalFiles);
	DDX_Check(pDX, IDC_CHECK_BACKUP_REMOTE_FILES, m_bBackupRemoteFiles);
}

BEGIN_MESSAGE_MAP(CRemoteDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteDlg)
	//ON_BN_CLICKED(IDC_ChangeDir, FTP_ChangeRemoteDirDown)
	ON_BN_CLICKED(IDC_BROWSE, OnLocalBrowse)
	ON_BN_CLICKED(IDC_GET, OnGetRemoteFile)
	ON_BN_CLICKED(IDC_PUT, OnPutRemoteFile)
	ON_BN_CLICKED(IDC_ASCII, OnAscii)
	ON_BN_CLICKED(IDC_BINARY, OnBinary)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteFile)
	ON_BN_CLICKED(IDC_RENAME, OnRenameFile)
	ON_BN_CLICKED(IDC_CREATEDIR, OnCreatedir)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(ID_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnBnClickedConnect)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_VIEWDIR, OnBnClickedButtonViewdir)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REMOTE, OnNMDblclkListRemote)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REMOTE, OnNMClickListRemote)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOCALDIR, OnNMDblclkListLocaldir)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOCALDIR, OnNMClickListLocaldir)
	ON_BN_CLICKED(IDC_BUTTON_REFRESHLOCAL, OnBnClickedButtonRefreshlocal)
	ON_BN_CLICKED(IDC_BUTTON_REFRESHREMOTE, OnBnClickedButtonRefreshremote)
	ON_BN_CLICKED(IDC_CREATEDIR_LOCAL, OnBnClickedCreatedirLocal)
	ON_BN_CLICKED(IDC_CHECK_KEEPALIVE, OnBnClickedCheckKeepalive)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg message handlers

BOOL CRemoteDlg::OnInitDialog() 
{
	CRect rect;
	CDialog::OnInitDialog();
	GetDlgItem(IDC_GET)->EnableWindow(FALSE);//Enables 
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RENAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CREATEDIR)->EnableWindow(FALSE);
	GetDlgItem(IDC_PUT)->EnableWindow(FALSE);//Disables

	((CButton *)GetDlgItem(IDC_BINARY))->SetCheck(1);

	// TODO: Add extra initialization here

	CAmazingFTP* pApp =(CAmazingFTP*)AfxGetApp();
	m_pImageList.Create(16, 16, ILC_COLOR8 | ILC_MASK,  9, 9);
	m_pImageList.Add(pApp->LoadIcon(IDI_FOLDERCLOSE));
	m_pImageList.Add(pApp->LoadIcon(IDI_CDUP));
	m_pImageList.Add(pApp->LoadIcon(IDI_EXEFILE));
	m_pImageList.Add(pApp->LoadIcon(IDI_FILE));
	m_pImageList.Add(pApp->LoadIcon(IDI_FILELINK));
	m_pImageList.Add(pApp->LoadIcon(IDI_FOLDERLINK));
	m_pImageList.Add(pApp->LoadIcon(IDI_LINK));
	m_pImageList.Add(pApp->LoadIcon(IDI_TEXTFILE));
	m_pImageList.Add(pApp->LoadIcon(IDI_ZIPFILE));
	m_pImageList.Add(pApp->LoadIcon(IDI_DRIVE));

	// assign image list
	m_RemoteListing.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_RemoteListing.SetImageList(&m_pImageList, LVSIL_SMALL);
	m_ctrlLocalListing.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_ctrlLocalListing.SetImageList(&m_pImageList, LVSIL_SMALL);

	// add headers
	m_RemoteListing.GetWindowRect(&rect);
	m_RemoteListing.SetHeadings( _T("Name,120;Size,50;Date,90;Permissions,80") );
	m_ctrlLocalListing.GetWindowRect(&rect);
	m_ctrlLocalListing.SetHeadings( _T("Name,120; Size,50; Date,120") );

	m_localPath = AfxGetApp()->GetProfileString(APP_SETTINGS_STRING, "LocalPath", "C:\\");
	m_ftpConnection.m_strLocalDirectory = m_localPath; // "c:\\";
	m_localWorkingDir = m_localPath;

	ShowLocalList(m_localPath.GetBuffer());
	m_ctrlStaticRemoteDir.SetWindowText("Remote Directory");

	return TRUE;  // return TRUE unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////
/////////////// Generic Application Button Handlers ///////////////
///////////////////////////////////////////////////////////////////

void CRemoteDlg::OnOK() 
{
	CDialog::OnOK();
}

void CRemoteDlg::OnCancel() 
{
	m_ftpConnection.Close();
	CDialog::OnCancel();
}

void CRemoteDlg::OnClose()
{
	OnCancel();
}

void CRemoteDlg::OnBnClickedCancel()
{
}

void CRemoteDlg::OnBnClickedExit()
{
	OnCancel();
}

void CRemoteDlg::OnHelpAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

///////////////////////////////////////////////////////////
////////// Application Specific Button Handlers ///////////
///////////////////////////////////////////////////////////

void CRemoteDlg::OnBnClickedConnect()
{
	CServerSetupDlg dlg;
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		m_serverName = dlg.m_serverName;
		m_password = dlg.m_password;
		m_remoteWorkingDir = dlg.m_remoteWorkingDir;
		m_localWorkingDir = dlg.m_localWorkingDir;
		m_userName = dlg.m_userName;

		int responseCode=0;

		AddToCmdTrail("Starting Connection ...");

		UpdateData(TRUE);
		//strcpy(cmdOpen, m_serverName.GetBuffer());

		if (m_remoteWorkingDir.IsEmpty())
			m_remoteWorkingDir = "/";

		responseCode = m_ftpConnection.Open(m_serverName.GetBuffer());
		AddToCmdTrail(m_ftpConnection.szBuffer);

		if(responseCode != INVALID_SOCKET)
		{
			if(responseCode != CFtp::SERVICE_READY) // 220 Service ready for new user. 
			{
				AddToCmdTrail("unable to connect to server");
				m_ftpConnection.Close();
			}
			else
			{
				char* GetBuffer();

				responseCode = m_ftpConnection.Login(m_userName.GetBuffer(), m_password.GetBuffer());
				if (responseCode != CFtp::USER_LOGGED_IN_PROCEED) // 230 User logged in, proceed. 
				{
					AddToCmdTrail("UserName/Password not correct");
					m_ftpConnection.Close();
				}
				else // Logged in so go !
				{
					AddToCmdTrail(m_ftpConnection.szBuffer);

					m_ftpConnection.GetSYST();
					AddToCmdTrail(m_ftpConnection.szBuffer);

					//////////OnBinary();

					m_localPath = m_localWorkingDir;
					ShowLocalList(m_localPath.GetBuffer());

					m_cstrSelectedFileName = m_remoteWorkingDir;
					FTP_ChangeRemoteDirDown();

					ShowRemoteList();
					///// IFF connected, change button name to "Disconnect"...
				}
			}
		}
		else
			AddToCmdTrail("Unable to Find FTP Server");
	}
}

////////////////////////////
// on Local ChgDir button //
////////////////////////////
void CRemoteDlg::OnLocalBrowse() 
{
	CFileDialog dlgFile(true);
	if (dlgFile.DoModal() == IDOK)
	{
		CString fileName = dlgFile.GetFileName();
		CString folderPath = dlgFile.GetPathName();
	}
}

// From REMOTE dir TO LOCAL dir
//////
void CRemoteDlg::OnGetRemoteFile() 
{
	m_ftpConnection.m_strLocalDirectory = m_localPath;
	if (int nSize = GetListSelections(REMOTE_LIST))
	{
		for (int n = 0; n < nSize; n++)
		{	
			int nRetCode = 0;
			CString csFileToTransfer = m_vcsFilesList.at(n);
			if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_FILE)
			{
				UpdateData(true);
				if (m_bBackupLocalFiles)
					AfxMessageBox("Would BACKUP LOCAL files ...");
				nRetCode = m_ftpConnection.GetFile(csFileToTransfer.GetBuffer());
			}
			else if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_DIR)
			{
				AddToCmdTrail("Transfer directory not enabled yet - check back!");
			}

			CString msg;   
			if (nRetCode == CFtp::FILE_ACTION_SUCCESSFUL)
				msg.Format("%s transferred", csFileToTransfer.GetBuffer());
			else
				msg.Format("Trouble transferring %s", csFileToTransfer.GetBuffer());
			AddToCmdTrail(msg.GetBuffer());
		}
	}
	ShowLocalList( m_localPath.GetBuffer());
}

// From LOCAL dir TO REMOTE dir
void CRemoteDlg::OnPutRemoteFile() 
{
	m_ftpConnection.m_strLocalDirectory = m_localPath;
	if (int nSize = GetListSelections(LOCAL_LIST))
	{
		for (int n = 0; n < nSize; n++)
		{	
			int nRetCode = 0;
			CString csFileToTransfer = m_vcsFilesList.at(n);
			if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_FILE)
			{
				UpdateData(true);
				if (m_bBackupRemoteFiles)
					AfxMessageBox("Would BACKUP REMOTE files ...");
				nRetCode = m_ftpConnection.PutFile(csFileToTransfer.GetBuffer()); // m_cstrSelectedFileName
			}
			else if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_DIR)
			{
				AddToCmdTrail("Transfer directory not enabled yet - check back!");
			}

			CString msg;   
			if (nRetCode == CFtp::FILE_ACTION_SUCCESSFUL)
				msg.Format("%s transferred", csFileToTransfer.GetBuffer());
			else
				msg.Format("Trouble transferring %s", csFileToTransfer.GetBuffer());
			AddToCmdTrail(msg.GetBuffer());
		}
	}
	ShowRemoteList(); // Update the directory listing.
}

void CRemoteDlg::OnAscii() 
{
	m_ftpConnection.Ascii();
	AddToCmdTrail("Transfer mode = ASCII");
}

void CRemoteDlg::OnBinary() 
{
	m_ftpConnection.Binary();
	AddToCmdTrail("Transfer mode = BINARY");
}

// Remote dir
void CRemoteDlg::OnDeleteFile() 
{
	if (int nSize = GetListSelections(REMOTE_LIST))
	{
		string strMsg = "Do You really want to Delete these files ?";
		for (int n = 0; n < nSize; n++)
			strMsg = strMsg + "\n\t" + m_vcsFilesList.at(n).GetBuffer();

		if (AfxMessageBox(strMsg.c_str(), MB_YESNO) == IDYES)
		{
			try
			{
				for (int n = 0; n < nSize; n++)
				{	
					int nRetCode;
					CString csFileToDelete = m_vcsFilesList.at(n);
					if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_FILE)
						nRetCode = m_ftpConnection.Delete(csFileToDelete.GetBuffer());
					else if (m_vcsFilesList.at(n).nIconType == SORTLIST_ICON_TYPE_DIR)
						nRetCode = m_ftpConnection.RMDir(csFileToDelete.GetBuffer());

					CString msg;
					if (nRetCode == CFtp::FILE_ACTION_COMPLETED)
						msg.Format(" %s Deleted", csFileToDelete.GetBuffer());
					else
						msg.Format("Trouble Deleting %s Deleted", csFileToDelete.GetBuffer());
					AddToCmdTrail(msg.GetBuffer());
				}
			}
			catch(...)
			{
				AfxMessageBox("Trouble deleting files!");
			}
			ShowRemoteList();
		}
	}
}

// Renaming Remote file in Dir 
void CRemoteDlg::OnRenameFile() 
{
	CUserInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString msg;

		CString renameCmd = m_cstrSelectedFileName + " " + dlg.m_FileName;
		msg.Format("Do You really want to rename %s to  %s?", m_cstrSelectedFileName.GetBuffer(), dlg.m_FileName.GetBuffer());
		
		if (AfxMessageBox(msg,MB_OKCANCEL) == IDOK)
		{
			m_ftpConnection.Move(renameCmd.GetBuffer());
			AddToCmdTrail(m_ftpConnection.szBuffer);
			ShowRemoteList();
		}
	}
}

// For creating Remote directory
void CRemoteDlg::OnCreatedir() 
{
	int responseCode = 0;
	CUserInputDlg dlg(1, NULL);
	if (dlg.DoModal() == IDOK)
	{
		responseCode = m_ftpConnection.MakeDir(dlg.m_FileName.GetBuffer());
		AddToCmdTrail(m_ftpConnection.szBuffer);

		if(responseCode == CFtp::PATHNAME_CREATED) // "PATHNAME" created.
		{
			AddToCmdTrail("Directory is sucessfully created");
			ShowRemoteList();
		}
	}
}

void CRemoteDlg::OnBnClickedButtonViewdir()
{
}

//////////////////////////////////////////////////
///////////////// Worker methods /////////////////
//////////////////////////////////////////////////
void CRemoteDlg::OnSize(UINT nType, int cx, int cy)
{
	//CDialog::OnSize(nType, cx, cy);
	// Put resizing logic here ...
}

///////////////////////////////////////////////////////////
// Updates the REMOTE directory listing in List Control. //
///////////////////////////////////////////////////////////
void CRemoteDlg::ShowRemoteList() 
{
	m_RemoteListing.DeleteAllItems();
	m_ftpConnection.RemoteList();

	m_RemoteListing.SetClassFolderType(2);
	m_RemoteListing.AddItem("","","","");

	basic_string <char>::size_type nPosEOL;
	static const basic_string <char>::size_type npos = -1;

	int nSentinel = 1000;
	while (m_ftpConnection.m_strDirListing.length() && nSentinel)
	{
		nPosEOL = m_ftpConnection.m_strDirListing.find((char)13);
		if ((nPosEOL != npos) && ((int)nPosEOL > 1))
		{
			string strLine = m_ftpConnection.m_strDirListing.substr(0, nPosEOL + 1);
			strLine.at(nPosEOL) = '\0';

			string strPermissions = GetNextToken(strLine);
			string strLnk = GetNextToken(strLine);
			string strOwner = GetNextToken(strLine);
			string strGrp = GetNextToken(strLine);
			string strSize = GetNextToken(strLine);
			string strDate1 = GetNextToken(strLine);
			string strDate2 = GetNextToken(strLine);
			string strDate3 = GetNextToken(strLine);
            string strName = strLine; // What's left IS the name!

			string strFullDate = strDate1 + " " + strDate2 + " " + strDate3;

			if(strPermissions.at(0) == 'd' )
				m_RemoteListing.SetClassFolderType(1); // Shows folder type graphic.
			else
				m_RemoteListing.SetClassFolderType(0);

			m_RemoteListing.AddItem(strName.c_str(), strSize.c_str(), strFullDate.c_str(), strPermissions.c_str());

			m_ftpConnection.m_strDirListing.erase(0, nPosEOL+2); // Delete line.
		}
		nSentinel--;
	}
	m_ftpConnection.m_strDirListing.clear();
	m_ctrlStaticRemoteDir.SetWindowText(m_remotePath.GetBuffer());
}

//////////////////////////////////////////////////////////
// Updates the LOCAL directory listing in List Control. //
//////////////////////////////////////////////////////////
void CRemoteDlg::ShowLocalList(char *path) 
{
	m_ctrlLocalListing.DeleteAllItems();
	m_ctrlLocalListing.SetClassFolderType(2); // Left "Up Arrow".
	m_ctrlLocalListing.AddItem("","","");

	CString csLocalPath = CString (path) + "\\*.*";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(csLocalPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString csFileLength;
		csFileLength.Format("%d", finder.GetLength());

		CTime tempTime;
		CString csWriteTime;
		if (finder.GetLastWriteTime(tempTime))
			csWriteTime = tempTime.Format(_T("%c"));
		else
			csWriteTime = "size=?";

		if (finder.IsDots())
			continue;

		if(finder.IsDirectory())
			m_ctrlLocalListing.SetClassFolderType(1);
		else
			m_ctrlLocalListing.SetClassFolderType(0);

		CString filename = finder.GetFileName();
		m_ctrlLocalListing.AddItem((LPCSTR)filename, (LPCSTR)csFileLength, (LPCSTR)csWriteTime);
	}
	m_ctrlStaticLocalDir.SetWindowText(m_localPath.GetBuffer());

	m_ctrlLocalListing.SetClassFolderType(3); // Drive icon.
	m_ctrlLocalListing.AddItem("C:\\","drive","");
	m_ctrlLocalListing.AddItem("D:\\","drive","");
}

//////////////////////////////////////////////////////////
// Calls FTP functions to change REMOTE directory DOWN. //
// ( Double clicking on directory ... )                 //
//////////////////////////////////////////////////////////
void CRemoteDlg::FTP_ChangeRemoteDirDown() 
{
	int responseCode=0;
	char cmdCD[200] = "";

	responseCode = m_ftpConnection.RemoteChangeDir(m_cstrSelectedFileName.GetBuffer());
	AddToCmdTrail(m_ftpConnection.szBuffer);

	if(responseCode != CFtp::FILE_ACTION_COMPLETED) // Requested file action okay, completed
	{
		AddToCmdTrail("can't change Directory");
		m_ftpConnection.PWD();
	}
	else
	{
		if (((m_remotePath.GetLength() > 1) || (m_remotePath.GetAt(0) != '/')) &&
			(m_cstrSelectedFileName.GetAt(0) != '/'))
			m_remotePath = m_remotePath + "/";
		m_remotePath = m_remotePath + m_cstrSelectedFileName.GetBuffer(); 
	}
	ShowRemoteList();
}

////////////////////////////////////////////////////////
// Calls FTP functions to change REMOTE directory UP. //
// ( Double clicking on directory ... )               //
////////////////////////////////////////////////////////
void CRemoteDlg::FTP_ChangeRemoteDirUP()
{
	char path[390];
	char * newpath=NULL;
	memset(path,0,390);
	int responseCode;
	responseCode = m_ftpConnection.CDUP();
	AddToCmdTrail(m_ftpConnection.szBuffer);

	if(responseCode != CFtp::FILE_ACTION_COMPLETED) // 250 Requested file action okay, completed. 
	{
		m_ftpConnection.PWD();
		AddToCmdTrail(m_ftpConnection.szBuffer);
	}
	else
	{
		int nSlash = m_remotePath.ReverseFind('/');
		if (nSlash > 0)
			m_remotePath.Truncate(nSlash);
		else if (nSlash == 0)
			m_remotePath.Truncate(1);
	}
	ShowRemoteList();
}

// Calls the FTP::KeepAlive method to send a NOOP to the server.
// Should return COMMAND_OKAY if all is well.
int CRemoteDlg::FTP_KeepAlive()
{
	int retCode = m_ftpConnection.KeepAlive();
	if (retCode == CFtp::COMMAND_OKAY)
		AddToCmdTrail("Keep Alive cmd sent");
	return retCode;
}

void CRemoteDlg::ChangeLocalDirDown() 
{
	CString csTemp = m_localPath;
	if (m_localPath.GetAt(m_localPath.GetLength()-1)!= '\\')
		csTemp = csTemp + '\\';
	
	csTemp = csTemp + m_cstrSelectedFileName;
	CFileFind finder;
	bool bStatus = finder.FindFile(csTemp);
	if (bStatus)
	{
		finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString csDisplay = csTemp + "\\*.*";
			ShowLocalList(csTemp.GetBuffer());
		}
	}
	m_localPath = csTemp;
}

void CRemoteDlg::ChangeLocalDirUP() 
{
	int nSlash = m_localPath.ReverseFind('\\');
	int nLength = m_localPath.GetLength();
	if (m_localPath.GetLength() > 3)
		m_localPath.Truncate(nSlash);

	if (m_localPath.GetLength() == 2)			// For C:\ D:\ etc ...
		m_localPath = m_localPath + "\\";

	ShowLocalList(m_localPath.GetBuffer());
}

// boolValue == TRUE enables Local buttons.
// boolValue == FALSE enables Remote buttons.
void CRemoteDlg::EnableLocalButtons(bool boolValue) 
{
	// Local buttons
	GetDlgItem(IDC_PUT)->EnableWindow(boolValue);
	GetDlgItem(IDC_BUTTON_REFRESHLOCAL)->EnableWindow(boolValue);

	// Remote buttons
	GetDlgItem(IDC_GET)->EnableWindow(!boolValue);
	GetDlgItem(IDC_DELETE)->EnableWindow(!boolValue);
	GetDlgItem(IDC_RENAME)->EnableWindow(!boolValue);
	GetDlgItem(IDC_CREATEDIR)->EnableWindow(!boolValue);
	GetDlgItem(IDC_BUTTON_REFRESHREMOTE)->EnableWindow(!boolValue);
}

void CRemoteDlg::AddToCmdTrail(const char* szString)
{
	char szTrailBuffer[100];
	strncpy(szTrailBuffer, szString, 95);
	szTrailBuffer[95] = '\0';
	try
	{
		sprintf(szTrailBuffer, "%s\r\n", szTrailBuffer);
		m_editCtrlCmdsTrail.ReplaceSel(szTrailBuffer);
	}
	catch (...)
	{
		sprintf(szTrailBuffer, "szString too big: $s $s\r\n", __FILE__, __LINE__);
		AfxMessageBox(szTrailBuffer);
	}
}

string CRemoteDlg::GetNextToken(string &strIn)
{
	static const basic_string <char>::size_type notFound = -1;
	basic_string <char>::size_type nPosEnd;
	nPosEnd = strIn.find(' ');
	if (nPosEnd > 0)
	{
		string strSubstring = strIn.substr(0, nPosEnd); // Substr up TO not including delimiter.
		strIn.erase(0, nPosEnd + 1);
		while (strIn.at(0) == ' ' || strIn.at(0) == '\t')
			strIn.erase(0, 1);

		return(strSubstring);
	}
	else
		return ("");
}

int CRemoteDlg::GetListSelections(int nWhichList) 
{
	CSortListCtrl* pSortListCtrl;
	if (nWhichList == 0)
		pSortListCtrl = &m_ctrlLocalListing;
	else
		pSortListCtrl = &m_RemoteListing;

	int nSelectedCount = pSortListCtrl->GetSelectedCount();
	if (nSelectedCount > 0)
	{
		m_vcsFilesList.clear(); // Get rid of any legacy filenames 

		POSITION pos = pSortListCtrl->GetFirstSelectedItemPosition();
		for (int nC = 0; nC < nSelectedCount; nC++)
		{
			if (pos) // change to while (pos)
			{
				int nItem = pSortListCtrl->GetNextSelectedItem(pos);
				Selection selNextItem;
				selNextItem.SetString(pSortListCtrl->GetItemText(nItem, 0));
				selNextItem.nIconType = pSortListCtrl->GetItemFolderType(nItem);
				m_vcsFilesList.push_back(selNextItem);
			}
		}
		nSelectedCount = m_vcsFilesList.size();
	}
	else
		nSelectedCount = 0;

	return nSelectedCount;
}

//////////////////////// REMOTE ////////////////////////
// We use Double click ONLY for changing directories! //
////////////////////////////////////////////////////////
void CRemoteDlg::OnNMDblclkListRemote(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iRow = pNMListView->iItem;
	m_cstrSelectedFileName = m_RemoteListing.GetItemText(iRow, 0);

	if(iRow == 0) // Top row so must want to move up one dir level.
		FTP_ChangeRemoteDirUP();
	else if(iRow > 0)
		FTP_ChangeRemoteDirDown();

	EnableLocalButtons(FALSE);
	*pResult = 0;
}

///////////////////////// REMOTE ////////////////////////
// Single click is used to select files & directories. //
/////////////////////////////////////////////////////////
void CRemoteDlg::OnNMClickListRemote(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iRow = pNMListView->iItem;

	m_cstrSelectedFileName = m_RemoteListing.GetItemText(iRow, 0);

	EnableLocalButtons(FALSE);
	*pResult = 0;
}

///////////////////////// LOCAL ////////////////////////
// We use Double click ONLY for changing directories! //
////////////////////////////////////////////////////////
void CRemoteDlg::OnNMDblclkListLocaldir(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iRow = pNMListView->iItem;
	m_cstrSelectedFileName = m_ctrlLocalListing.GetItemText(iRow, 0);

	int nItemData = static_cast<int> (m_ctrlLocalListing.GetItemData(iRow));

	if (nItemData == 3)
	{
		m_localPath = m_cstrSelectedFileName;
		ShowLocalList(m_localPath.GetBuffer());
	}
	else
		if (nItemData != 0)
		{
			if (iRow == 0) // Top row so must want to move up one dir level.
				ChangeLocalDirUP();
			else if (iRow > 0)
				ChangeLocalDirDown();

			m_ctrlStaticLocalDir.SetWindowText(m_localPath.GetBuffer());
			EnableLocalButtons(TRUE);
		}
	*pResult = 0;
}

////////////////////////// LOCAL ////////////////////////
// Single click is used to select files & directories. //
/////////////////////////////////////////////////////////
void CRemoteDlg::OnNMClickListLocaldir(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iRow = pNMListView->iItem;

	m_cstrSelectedFileName = m_ctrlLocalListing.GetItemText(iRow, 0);

	EnableLocalButtons(TRUE);
	*pResult = 0;
}

void CRemoteDlg::OnBnClickedButtonRefreshlocal()
{
	ShowLocalList(m_localPath.GetBuffer());
}

void CRemoteDlg::OnBnClickedButtonRefreshremote()
{
	ShowRemoteList();
}

void CRemoteDlg::OnBnClickedCreatedirLocal()
{
	int responseCode = 0;
	CUserInputDlg dlg(2, NULL);
	CString csFolderName;

	if (dlg.DoModal() == IDOK)
	{
		csFolderName = dlg.m_FileName.GetBuffer();
		if (!csFolderName.IsEmpty())
		{
			_chdir(m_localWorkingDir.GetBuffer());
			int retCode = _mkdir(csFolderName.GetBuffer());
			if (retCode == 0)
			{
				AddToCmdTrail( "Local Directory created" );
				ShowLocalList(m_localPath.GetBuffer());
			}
		}
		else
			AddToCmdTrail( "Problem creating directory." );
	}
}

void CRemoteDlg::OnBnClickedCheckKeepalive()
{	UpdateData();
	if (m_ctrlCheckKeepAlive)
	{
		AddToCmdTrail("KeepAlive is enabled.");
		AfxBeginThread(WorkerThread, this);
	}
	else
		AddToCmdTrail("KeepAlive was disabled.");
}

/////
// NEED to use CSemaphore or CCriticalSection here to access a member variable.
// make it protected. Set it ONLY from the KeepAlive checkbox call.

UINT WorkerThread(LPVOID pParam)
{
    CRemoteDlg* pObject = (CRemoteDlg*)pParam;

    if (pObject == NULL) // || !pObject->IsKindOf(RUNTIME_CLASS(CRemoteDlg)))
		return 1;   // if pObject is not valid

	do
	{
		pObject->FTP_KeepAlive();
		Sleep(pObject->m_nKeepAlivePeriodSeconds * 1000);
	} while (pObject->m_ctrlCheckKeepAlive);

    return 0;   // thread completed successfully
}
