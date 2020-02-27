#if !defined(AFX_TRACEDLG_H__B6E34344_27A5_11D2_80EE_006008BFC7B0__INCLUDED_)
#define AFX_TRACEDLG_H__B6E34344_27A5_11D2_80EE_006008BFC7B0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TraceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TraceDlg dialog

class TraceDlg : public CDialog
{
// Construction
public:
	TraceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TraceDlg)
	enum { IDD = IDD_DIALOG1 };
	CListBox	m_ctrlList1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TraceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TraceDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEDLG_H__B6E34344_27A5_11D2_80EE_006008BFC7B0__INCLUDED_)
