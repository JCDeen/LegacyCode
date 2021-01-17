#if !defined(AFX_DIGITALOUTPUT_H__AAF8C626_3489_49B3_B668_CFF5A2DA5DDA__INCLUDED_)
#define AFX_DIGITALOUTPUT_H__AAF8C626_3489_49B3_B668_CFF5A2DA5DDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigitalOutput.h : header file
//

#include "stdafx.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CDigitalOutput command target

class CDigitalOutput : public CSocket
{
// Attributes
public:
	UINT	m_nPortNumber;
	CString	m_csPassword;
	CString	m_csIPAddress;
	bool m_bSocketIsConnected;

	char m_szAnalogValArray[16][10];


// Operations
public:
	CDigitalOutput();
	virtual ~CDigitalOutput();

	void ProcessSendResponse(int nReturnCode, int nBufSize, char *stringOK, char *stringError);
	void OutputDigitalValue(const char *szPtrString);
	void CDigitalOutput::EchoStatus(char *szTextString);



// Overrides
public:
	bool ConnectToRobot();
	void testFunction();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitalOutput)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDigitalOutput)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALOUTPUT_H__AAF8C626_3489_49B3_B668_CFF5A2DA5DDA__INCLUDED_)
