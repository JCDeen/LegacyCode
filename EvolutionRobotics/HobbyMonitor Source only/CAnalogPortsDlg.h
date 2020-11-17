#if !defined(AFX_CANALOGPORTSDLG_H__EC8047AB_ED47_4553_88F7_FDF67452EF0E__INCLUDED_)
#define AFX_CANALOGPORTSDLG_H__EC8047AB_ED47_4553_88F7_FDF67452EF0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAnalogPortsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnalogPortsDlg dialog

class CAnalogPortsDlg : public CDialog
{
// Construction
public:
	CAnalogPortsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnalogPortsDlg)
	enum { IDD = IDD_DIALOG_ANALOG_PORTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalogPortsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnalogPortsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANALOGPORTSDLG_H__EC8047AB_ED47_4553_88F7_FDF67452EF0E__INCLUDED_)
