// TraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IsapiServer.h"
#include "TraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TraceDlg dialog


TraceDlg::TraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TraceDlg)
	//}}AFX_DATA_INIT
}


void TraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TraceDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctrlList1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TraceDlg, CDialog)
	//{{AFX_MSG_MAP(TraceDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TraceDlg message handlers
