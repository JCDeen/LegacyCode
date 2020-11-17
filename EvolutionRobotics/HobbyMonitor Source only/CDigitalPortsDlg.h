#if !defined(AFX_CDIGITALPORTSDLG_H__BCAC5726_5345_4BA0_BA60_7953E71AE121__INCLUDED_)
#define AFX_CDIGITALPORTSDLG_H__BCAC5726_5345_4BA0_BA60_7953E71AE121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDigitalPortsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDigitalPortsDlg dialog

class CDigitalPortsDlg : public CDialog
{
// Construction
public:
	CDigitalPortsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDigitalPortsDlg)
	enum { IDD = IDD_DIALOG_DIGITALPORTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitalPortsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDigitalPortsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIGITALPORTSDLG_H__BCAC5726_5345_4BA0_BA60_7953E71AE121__INCLUDED_)
