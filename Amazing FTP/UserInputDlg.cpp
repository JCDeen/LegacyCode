// UserInputDlg.cpp : implementation file
//
///// Gets Filename DIALOG 
//

#include "stdafx.h"
#include "amazingFTP.h"
#include "UserInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg dialog


CUserInputDlg::CUserInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInputDlg)
	m_FileName = _T("");
	m_flag=0;
	//}}AFX_DATA_INIT
}

CUserInputDlg::CUserInputDlg(int flag, CWnd* pParent /*=NULL*/)
	: CDialog(CUserInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInputDlg)
	m_FileName = _T("");
	m_flag = flag;
	//}}AFX_DATA_INIT
}

void CUserInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInputDlg)
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInputDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg message handlers

BOOL CUserInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_flag == 1)
	{
		GetDlgItem(IDC_STATIC)->SetWindowText("Name of Folder to be created :");
		this->SetWindowText("Create Remote Directory");

	}
	else if (m_flag == 2)
	{
		GetDlgItem(IDC_STATIC)->SetWindowText("Name of Folder to be created :");
		this->SetWindowText("Create Local Directory");
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	////CAmazingFTP* app =(CAmazingFTP*) AfxGetApp(); 
	////app->setFileName(m_FileName);
	//AfxMessageBox("FORMERLY app->setFileName(m_FileName)");
	
	CDialog::OnOK();
}
