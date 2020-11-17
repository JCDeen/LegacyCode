// CAnalogPortsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "CAnalogPortsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnalogPortsDlg dialog


CAnalogPortsDlg::CAnalogPortsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalogPortsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnalogPortsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAnalogPortsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalogPortsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnalogPortsDlg, CDialog)
	//{{AFX_MSG_MAP(CAnalogPortsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalogPortsDlg message handlers
