// mfcClient.h : main header file for the MFCCLIENT application
//

#if !defined(AFX_AMAZING_FTP_H__BC671989_4509_499A_8F91_C6DDE339C04B__INCLUDED_)
#define AFX_AMAZING_FTP_H__BC671989_4509_499A_8F91_C6DDE339C04B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ftp.h"
#include "RemoteDlg.h"

#define APP_SETTINGS_STRING "AppSettings"
/////////////////////////////////////////////////////////////////////////////
// CAmazingFTP:
// See amazingFTP.cpp for the implementation of this class
//

class CAmazingFTP : public CWinApp
{
//		CDataManager * m_pDataManager;
	
public:
	CAmazingFTP();
	//CDataManager * CAmazingFTP::getDataManager()
	//{
	//	return m_pDataManager;
	//}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmazingFTP)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAmazingFTP)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CFtp m_theFtpConnection;
	//static CDataManager * _instance;
	CString m_Dir;
	CString m_FileToHandle;
	CString m_SelectedFile;
	int		m_SelectFilesize;
	CRemoteDlg * m_pRemoteDlg;

public:
	//CDataManager();
	//static CDataManager * getInstance();
	//virtual ~CDataManager();
	//void setRemoteDlg(CRemoteDlg *pRemoteDlg);
	CRemoteDlg * getRemoteDlg();
	CFtp * getftpconnection();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMAZING_FTP_H__BC671989_4509_499A_8F91_C6DDE339C04B__INCLUDED_)
