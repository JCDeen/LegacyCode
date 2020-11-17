#if !defined(AFX_CRemoteDlg_H__84423D1E_401B_4998_B546_F8C83AFCAA47__INCLUDED_)
#define AFX_CRemoteDlg_H__84423D1E_401B_4998_B546_F8C83AFCAA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRemoteDlg.h : header file
//

#include "SortListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog

UINT WorkerThread(LPVOID pParam);

class CRemoteDlg : public CDialog
{
// Construction
	char *m_pPath;

	static int nCounter;

public:
	CRemoteDlg(CWnd* pParent = NULL);   // standard constructor

	void FTP_ChangeRemoteDirUP();
	void FTP_ChangeRemoteDirDown();
	int FTP_KeepAlive();

	void ChangeLocalDirDown(); 
	void ChangeLocalDirUP(); 

	void ShowRemoteList();
	void ShowLocalList(char *path);

	void EnableLocalButtons(bool boolValue);

	// Dialog Data
	//{{AFX_DATA(CRemoteDlg)
	enum { IDD = IDD_DIALOG1 };
	CProgressCtrl	m_progressbar;
	CSortListCtrl	m_RemoteListing;
	CSortListCtrl   m_ctrlLocalListing;
	//}}AFX_DATA

	/** Strings for server connection. **/
	CString m_serverSetupName;
	CString	m_serverName;
	CString	m_password;
	CString	m_remoteWorkingDir;
	CString	m_localWorkingDir;
	CString	m_userName;

	//int m_nbIsRemoteDir;
	//////CFtp *m_ftpConnection;
	CFtp m_ftpConnection;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CImageList  m_pImageList;
	// Generated message map functions
	//{{AFX_MSG(CRemoteDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLocalBrowse();
	afx_msg void OnGetRemoteFile();
	afx_msg void OnPutRemoteFile();
	afx_msg void OnAscii();
	afx_msg void OnBinary();
	virtual void OnCancel();
	afx_msg void OnDeleteFile();
	afx_msg void OnRenameFile();
	afx_msg void OnCreatedir();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnHelpAbout();
	CEdit m_editCtrlCmdsTrail; // Make protected

	CString m_cstrSelectedFileName;
	CString m_cstrFileName;
	CString	m_localPath;
	CString	m_remotePath;

	struct Selection : public CString
	{
		int nIconType;
	};

	vector <Selection> m_vcsFilesList;

	void AddToCmdTrail(const char* szString);

	afx_msg void OnBnClickedButtonViewdir();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_ctrlStaticRemoteDir;
	CStatic m_ctrlStaticLocalDir;

	string GetNextToken(string& strIn); //, string strToken=" ");
	int GetListSelections(int nWhichList=0); // 0==LocalList; 1==RemoteList
	#define LOCAL_LIST 0
	#define REMOTE_LIST 1

	afx_msg void OnNMDblclkListRemote(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListRemote(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListLocaldir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListLocaldir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRefreshlocal();
	afx_msg void OnBnClickedButtonRefreshremote();
	afx_msg void OnBnClickedCreatedirLocal();
	afx_msg void OnBnClickedCheckKeepalive();

	int m_nKeepAlivePeriodSeconds;
	BOOL m_ctrlCheckKeepAlive;
	BOOL m_bBackupLocalFiles;
	BOOL m_bBackupRemoteFiles;

	BOOL m_bIsConnected;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRemoteDlg_H__84423D1E_401B_4998_B546_F8C83AFCAA47__INCLUDED_)
