#if !defined(AFX_DIGITALPORTSDLG_H__F65F824E_4FCE_4F59_860A_16C2C5EAB364__INCLUDED_)
#define AFX_DIGITALPORTSDLG_H__F65F824E_4FCE_4F59_860A_16C2C5EAB364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigitalPortsDlg.h : header file
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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALPORTSDLG_H__F65F824E_4FCE_4F59_860A_16C2C5EAB364__INCLUDED_)
