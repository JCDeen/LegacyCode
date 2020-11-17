#if !defined(AFX_USERINPUTDLG_H__8A536E96_6DB8_4FAE_9E26_B13C176AED31__INCLUDED_)
#define AFX_USERINPUTDLG_H__8A536E96_6DB8_4FAE_9E26_B13C176AED31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNameHandeler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserInputDlg dialog

class CUserInputDlg : public CDialog
{
// Construction
	int m_flag;
public:
	CUserInputDlg(CWnd* pParent = NULL);   // standard constructor
	CUserInputDlg(int flag,CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CUserInputDlg)
	enum { IDD = IDD_RENFILENAME };
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMEHANDELER_H__8A536E96_6DB8_4FAE_9E26_B13C176AED31__INCLUDED_)
