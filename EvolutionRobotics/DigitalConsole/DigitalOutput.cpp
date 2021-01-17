// DigitalOutput.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalConsole.h"
#include "DigitalOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitalOutput

CDigitalOutput::CDigitalOutput()
{
	this->Create();

	
	m_bSocketIsConnected = false;
	m_nPortNumber = 9000;
	m_csPassword = _T("Active in Version 1.0");
	m_csIPAddress = _T("127.0.0.1");


	/*
	if (Create())
		EchoStatus("In constructor ... Create() called OK");
	else
		EchoStatus("In constructor ... Create() had problem");
*/
}

CDigitalOutput::~CDigitalOutput()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDigitalOutput, CSocket)
	//{{AFX_MSG_MAP(CDigitalOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDigitalOutput member functions

#define CONNECTED 1
#define DISCONNECTED 2

bool CDigitalOutput::ConnectToRobot()
{	static int nConnectionState = DISCONNECTED;

	if (Connect(m_csIPAddress.GetBuffer(15), m_nPortNumber))
	{
		EchoStatus("Connected to robot successfully!");
		m_bSocketIsConnected = true;
		nConnectionState = CONNECTED;

	}
	else
	{
		EchoStatus("Could not connect to ER-1 Robot...");
		EchoStatus("Closing created socket.");
		Close();
		m_bSocketIsConnected = false;
		nConnectionState = DISCONNECTED;
	}
	return (m_bSocketIsConnected);
}

void CDigitalOutput::EchoStatus(char *szTextString)
{
	cout << szTextString << endl;
}


#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08

#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

void CDigitalOutput::OutputDigitalValue(const char *szPtrString)
{
	char szBuffer[9];
	strncpy(szBuffer, szPtrString, 8);
	szBuffer[8] = '\0';

	int nDigitalOutLO = 0, nDigitalOutHI = 0;
	
	nDigitalOutHI = (szBuffer[0] == '1') ? nDigitalOutHI | BIT_3 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[1] == '1') ? nDigitalOutHI | BIT_2 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[2] == '1') ? nDigitalOutHI | BIT_1 : nDigitalOutHI;
	nDigitalOutHI = (szBuffer[3] == '1') ? nDigitalOutHI | BIT_0 : nDigitalOutHI;

	nDigitalOutLO = (szBuffer[4] == '1') ? nDigitalOutLO | BIT_3 : nDigitalOutLO;;
	nDigitalOutLO = (szBuffer[5] == '1') ? nDigitalOutLO | BIT_2 : nDigitalOutLO;
	nDigitalOutLO = (szBuffer[6] == '1') ? nDigitalOutLO | BIT_1 : nDigitalOutLO;
	nDigitalOutLO = (szBuffer[7] == '1') ? nDigitalOutLO | BIT_0 : nDigitalOutLO;

	char szDigitalOutHexValHI[2], szDigitalOutHexValLO[2];
	szDigitalOutHexValHI[1] = '\0';
	szDigitalOutHexValLO[1] = '\0';

	_itoa(nDigitalOutLO, szDigitalOutHexValLO, 16);
	_itoa(nDigitalOutHI, szDigitalOutHexValHI, 16);

	char szCmd[50];
	sprintf(szCmd, "output digital %c%c\n\r",toupper(szDigitalOutHexValHI[0]), toupper(szDigitalOutHexValLO[0]));

	int nErrCode = Send(szCmd, strlen(szCmd));

	/***************
	if (m_bEchoAppDebugging)
	{
		if (nErrCode == SOCKET_ERROR)
			EchoStatus("SOCKET_ERROR");
		else
		{	char chResponse[201];
			int nRet = Receive(chResponse, 150);
			ProcessSendResponse(nRet, 150, "Dig Out Sent OK", "ERR Receiving Dig Out response");
		}
	}
	******************/

	EchoStatus(szCmd);
}


void CDigitalOutput::ProcessSendResponse(int nReturnCode, int nBufSize, char *stringOK, char *stringError)
{
	if (nReturnCode == SOCKET_ERROR)
		TRACE("ProcessSendResponse: nReturnCode == SOCKET_ERROR");

	if (nReturnCode > 0 && nReturnCode < nBufSize && nReturnCode != SOCKET_ERROR)
		EchoStatus(stringOK);
	else
		EchoStatus(stringError);
}

//////////////////////////

void CDigitalOutput::testFunction()
{
	cout << "testFunction() was called" << endl;
}


