// DigitalConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DigitalConsole.h"
#include "DigitalOutput.h"

#include <conio.h>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	bool bWaitForKeystroke = false; // false;
	CString csBinaryOutputValue = "";
	int nRetCode = 0;
	CDigitalOutput digitalOut;

	/////////////////////////////////////
	// Process command line arguments. //
	/////////////////////////////////////
	if (argc == 1)
	{	cout << "Usage:" << endl;
		cout << "DigitalConsole.exe 10011101 [wait]" << endl;
		cout << ".................................." << endl;
		cout << "Note: binary argument must be 8 characters." << endl;
		cout << "optional 'wait' argument waits for any keystroke before shutting down program." << endl;
		return nRetCode;
	}
	else
	{
		csBinaryOutputValue = argv[1];				// Assume 1st argument is binary value to set.
		if (argc >= 3) bWaitForKeystroke = true;	// Assume 2nd argument is "Wait".
	}

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		//////////////////////////
		// Verify binary value. //
		//////////////////////////
		if (csBinaryOutputValue.GetLength() != 8)
		{	cout << "ERROR: Binary value must be 8 characters." << endl;
			cout << "       length = " << csBinaryOutputValue.GetLength() << endl;
		}
		else
		{
			// cout << "Binary value is [" << csBinaryOutputValue.GetBuffer(8) << "]" << endl;
		}
		//////////////////////////////////////////////////////
		// Assumptions:                                     //
		//	1) we have a valid binary value to set.         //
		//	2) IP = 127.0.0.1 and port = 9000, no password. //
		//////////////////////////////////////////////////////
		if (AfxSocketInit())
		{	//CDigitalOutput digitalOut;
			digitalOut.Create();

			cout << "m_csIPAddress: " << digitalOut.m_csIPAddress.GetBuffer(18) << endl;
			cout << "m_nPortNumber: " << digitalOut.m_nPortNumber << endl;
			digitalOut.ConnectToRobot();
			digitalOut.OutputDigitalValue(csBinaryOutputValue);

			Sleep(300);

		}
	}

	/////////////////////////////////////////////////////
	// If there is a 2nd argument, assume it is "Wait" //
	// & wait for any keystroke before ending program. //
	/////////////////////////////////////////////////////
	if (bWaitForKeystroke)
	{
		cout << "Press any key to continue ..." << endl;
		getch();
	}

	digitalOut.Close();
	return nRetCode;
}


///////////////
// SCRATCH area:
/*******
		for (int n=0; n<argc; n++)
			cout << "Arg[" << n << "] = " << argv[n] << endl;
********/

