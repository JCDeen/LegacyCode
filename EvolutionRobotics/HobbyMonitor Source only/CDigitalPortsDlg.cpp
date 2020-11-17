// CDigitalPortsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "CDigitalPortsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitalPortsDlg dialog


CDigitalPortsDlg::CDigitalPortsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitalPortsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDigitalPortsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDigitalPortsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDigitalPortsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDigitalPortsDlg, CDialog)
	//{{AFX_MSG_MAP(CDigitalPortsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

