// ISAPISERVER.CPP - Implementation file for your Internet Server
//    IsapiServer Extension

#include "stdafx.h"
#include "IsapiServer.h"
#include "WIP.h"
#include "Config.h"
#include "WaferClient.h"
#include "MFTransaction.h"

WaferConfig* gpstWaferConfig;

CWinApp theApp;

///////////////////////////////////////////////////////////////////////
// command-parsing map

BEGIN_PARSE_MAP(CIsapiServerExtension, CHttpServer)
	ON_PARSE_COMMAND(Default, CIsapiServerExtension, ITS_EMPTY)
	DEFAULT_PARSE_COMMAND(Default, CIsapiServerExtension)
	ON_PARSE_COMMAND(Login, CIsapiServerExtension, ITS_PSTR ITS_PSTR ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("U P S")
	ON_PARSE_COMMAND(Header, CIsapiServerExtension, ITS_PSTR ITS_PSTR ITS_PSTR )
	//ON_PARSE_COMMAND_PARAMS("ClientID") // Don't use for Header function
	ON_PARSE_COMMAND(GetPage, CIsapiServerExtension, ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("ClientID")
	ON_PARSE_COMMAND(Expert, CIsapiServerExtension, ITS_PSTR ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("C ClientID")
	ON_PARSE_COMMAND(Logout, CIsapiServerExtension, ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("ClientID")

	ON_PARSE_COMMAND(Admin, CIsapiServerExtension, ITS_PSTR ITS_PSTR ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("ClientID=~ AdminPW=~ AdminType=~")

	ON_PARSE_COMMAND(Test, CIsapiServerExtension, ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR
												  ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR
												  ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR ITS_PSTR
												  ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("E1=~ E2=~ E3=~ E4=~ E5=~ E6=~ E7=~ E8=~ E9=~ E10=~ "
							"E11=~ E12=~ E13=~ E14=~ E15=~ E16=~ E17=~ E18=~ E19=~ E20=~ " 
							"E21=~ E22=~ E23=~ E24=~ E25=~ E26=~ E27=~ E28=~ E29=~ E30=~ E31=~")
END_PARSE_MAP(CIsapiServerExtension)

// JCD MAke AdminType Integer ?


///////////////////////////////////////////////////////////////////////
// The one and only CIsapiServerExtension object

CIsapiServerExtension theExtension;


///////////////////////////////////////////////////////////////////////
// CIsapiServerExtension implementation

CIsapiServerExtension::CIsapiServerExtension()
{
	m_bInitOK = FALSE;		// Changed if GetExtensionVersion() returns TRUE.
	m_nChildProcesses = 0;	// Not used at present.
	m_bDeleteInactiveClients = TRUE;
	m_nSecondsToSleep =	300;

}

CIsapiServerExtension::~CIsapiServerExtension()
{	TRACE("::~CIsapiServerExtension()\r\n");
}

BOOL CIsapiServerExtension::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// Call default implementation for initialization
	CHttpServer::GetExtensionVersion(pVer);

	// Load description string
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(), IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN));
	_tcscpy(pVer->lpszExtensionDesc, sz);

	m_nAppListeningPort = theApp.GetProfileInt("Wafer Server Settings", "Application Listening Port", -1);
	m_nStartPID = theApp.GetProfileInt("Wafer Server Settings", "Start PID", -1);
	m_nStopPID = theApp.GetProfileInt("Wafer Server Settings", "Stop PID", -1);

	m_nInactivityTimer = theApp.GetProfileInt("Wafer Server Attributes", "Inactivity Timer", -1);
	m_csInactivityTimerUnits = theApp.GetProfileString("Wafer Server Attributes", "Inactivity Timer Units", "Minutes");
	m_csHTMLTemplateDir = theApp.GetProfileString("Wafer Server Attributes", "HTML Templates Directory", "NOT FOUND");
	m_nSecondsToSleep = theApp.GetProfileInt("Wafer Server Attributes", "Cleaner Sleep Interval", 300);
	m_bDeleteInactiveClients = theApp.GetProfileInt("Wafer Server Attributes", "Enable Cleaner Thread", 1);

	m_csDCPIPAddress = theApp.GetProfileString("Mainframe DCP", "DCP IP Address", "57.192.50.3");
	m_nDCPPortNumber= theApp.GetProfileInt("Mainframe DCP", "DCP Port Number", 23);

	m_nTimesRun = theApp.GetProfileInt("Logging", "Times Run", -1);
	theApp.WriteProfileInt("Logging", "Times Run", m_nTimesRun+1);

	gpstWaferConfig = new WaferConfig();

	// Very redundant here, optimize later ...
	gpstWaferConfig->lBeginPID = m_nStartPID ;
	gpstWaferConfig->lEndPID = m_nStopPID;
	gpstWaferConfig->lTimeout = m_nInactivityTimer;
	strcpy(gpstWaferConfig->szTransServerAddress, m_csDCPIPAddress);
	gpstWaferConfig->nTransServerPort = m_nDCPPortNumber;

	m_pClientManager = new ClientManager(m_nInactivityTimer*60);

	if (m_pClientManager->Initialize() == FALSE)
	{
		TRACE(" CIsapiServerExtension::GetExtensionVersion - Unable to initialize the Wafer server\n");
		return FALSE;
	}

	if(m_bDeleteInactiveClients != 0)
		AfxBeginThread(CleanerFunction, this);

	m_bInitOK = TRUE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////
// CIsapiServerExtension command handlers

// Make this callable from other functions ... JCD

void CIsapiServerExtension::Default(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	ShowINIValues(pCtxt);

	EndContent(pCtxt);
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CIsapiServerExtension, CHttpServer)
	//{{AFX_MSG_MAP(CIsapiServerExtension)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0



///////////////////////////////////////////////////////////////////////
// If your extension will not use MFC, you'll need this code to make
// sure the extension objects can find the resource handle for the
// module.  If you convert your extension to not be dependent on MFC,
// remove the comments arounn the following AfxGetResourceHandle()
// and DllMain() functions, as well as the g_hInstance global.

/****

static HINSTANCE g_hInstance;

HINSTANCE AFXISAPI AfxGetResourceHandle()
{
	return g_hInstance;
}

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ulReason,
					LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInst;
	}

	return TRUE;
}

****/


void CIsapiServerExtension::Login(CHttpServerContext * pCtxt, LPTSTR pstrUserID, LPSTR pstrPassword,
							  LPSTR pstrSystem)
{
	BOOL bGotClientID;
	CString csClientID;

	LoginRequestData stLoginRequest;

	DWORD dwSize = 1000;
	char *buf = (char *)calloc(dwSize, sizeof(char));
	pCtxt->GetServerVariable("REMOTE_ADDR", buf, &dwSize);

	strcpy(stLoginRequest.szUserId, pstrUserID);
	strcpy(stLoginRequest.szPassword, pstrPassword);
	strcpy(stLoginRequest.szSystem, pstrSystem);
	strcpy(stLoginRequest.szIPAddress, buf);

	dwSize = 1000;
	pCtxt->GetServerVariable("HTTP_USER_AGENT", buf, &dwSize); // Use to get the browser type.

	free(buf);

	char szClientID[MAX_CLIENTID_LEN+1];
	strcpy(szClientID, "");

	if (ProcessRequest(LOGIN_REQUEST, szClientID, &stLoginRequest) == NO_WAFER_ERROR)
	{

		////////////////////////////////////////////////////////////////////////////////
		// JCD - Should TAKE THIS OUT LATER. Should be handled sooner than here ! JCD //
		////////////////////////////////////////////////////////////////////////////////
		if(m_pClientManager->VerifyClient(szClientID) == NULL)
		{
			StartContent(pCtxt);
			WriteTitle(pCtxt);

			*pCtxt << "<H2>VerifyClient(" << szClientID << ")== NULL)<BR>\r\n";

			CString cs;
			cs.Format("Has %d chars",strlen(szClientID));
			*pCtxt << cs;

			EndContent(pCtxt);
		}
		else
		{

			*pCtxt << "<!DOCTYPE HTML PUBLIC \"-//W3O//DTD W3 HTML 3.0//EN\">";

			*pCtxt << "<HTML>\r\n";
			*pCtxt << "<HEAD>\r\n";
			*pCtxt << "<TITLE>WAFER</TITLE>\r\n";
			*pCtxt << "</HEAD>\r\n";

			*pCtxt << "<frameset rows=\"40%, *\" border=0 frameborder=0>\r\n";
			*pCtxt << "<frame src=\"/cgi-bin/IsapiServer.dll?GetPage&ClientID=" << szClientID; //csClientID;
			*pCtxt << "\" name=\"Header\" scrolling=\"AUTO\" border=0 frameborder=0></frame> \r\n";

			*pCtxt <<  "<frame src=\"welcome.htm\" name=\"Main\" scrolling=\"AUTO\" border=0 frameborder=0</frame>\r\n";
			*pCtxt <<  "<noframes>\r\n";
			*pCtxt << "<BODY>\r\n";
			*pCtxt << "    <p>This web page uses frames, but your browser doesn't support them.</p>\r\n";
			*pCtxt << "</BODY>\r\n";
			*pCtxt << "    </noframes>\r\n";
			*pCtxt << "</frameset>\r\n";
			*pCtxt << "</HTML>";
		}
	}
	else
	{
		StartContent(pCtxt);
		WriteTitle(pCtxt);

		*pCtxt << "<H1>Unable to Login for :</H1>\n";
		*pCtxt << "pstrSystem  : " << stLoginRequest.szSystem   <<"<BR>\n";
		*pCtxt << "pstrUserID  : " << stLoginRequest.szUserId   <<"<BR>\n";
		*pCtxt << "pstrPassword: " << stLoginRequest.szPassword <<"<BR>";

		EndContent(pCtxt);
	}
}

//
// NOTE:	ON_PARSE_COMMAND_PARAMS macro is NOT used for Header function, so we have to do some parsing on our own.
//
void CIsapiServerExtension::Header(CHttpServerContext *pCtxt, LPTSTR pstrClientID, LPTSTR pstrXValue, LPTSTR pstrYValue)
{
	// Last 2 parms are INPUT TYPE=IMAGE X & Y values, but we'll
	// just parse the X Value text to get the name of the service requested.
	CString csService(pstrXValue), csClientID(pstrClientID);
 
	// Needed because no ON_PARSE_COMMAND_PARAMS for Header function.
	csClientID = csClientID.Right(csClientID.GetLength()-9);

	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << "csClientID " << csClientID;
	switch (pstrXValue[0])
	{	case 'E': // Expert
			*pCtxt << "<CENTER><H2>winfare - Expert Mode</H2></CENTER><P>\r\n";
			*pCtxt << "<FORM ACTION=\"/cgi-bin/IsapiServer.dll?Expert\" METHOD=\"POST\">\r\n";
			*pCtxt << "<B>Please enter an Expert Mode command:</B>\r\n";
			*pCtxt << "    	<INPUT TYPE=TEXT NAME=C VALUE=\"DBWWAF NYC ATL DL\" size=40 maxlength=100>\r\n";
			*pCtxt << "    	<INPUT TYPE=SUBMIT>\r\n";
			*pCtxt << "</CENTER></P>\r\n ";
			*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"ClientID\" VALUE=\"" << csClientID << "\">\r\n";
			*pCtxt << "</FORM>";
			break;

		case 'L': // Logoff
			*pCtxt << "<CENTER><H2>\r\n";
			*pCtxt << "If you are sure you want to logoff press the button below.<BR>\r\n";
			*pCtxt << "Otherwise, use the \"Back\" button on your browser,or select a different Winfare service above.\r\n";
			*pCtxt << "<form action=\"/cgi-bin/IsapiServer.dll?Logout\" method=\"POST\" TARGET=\"Main\">\r\n";
			*pCtxt << "    <P><input type=\"submit\" value=\"Logoff\">\r\n";
			*pCtxt << "    </P>\r\n";
			*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"ClientID\" VALUE=\"" << csClientID << "\">\r\n";
			*pCtxt << "</form>\r\n";
			*pCtxt << "</CENTER>";
			break;

		case 'F': // Fares
		case 'R': // Rules
		case 'N': // Footnotes
		case 'U': // Routings
		case 'A': // Addons
		case 'G': // Globals
		case 'Z': // Zones
		default :
			*pCtxt << "<BR>Service selected was : " << pstrXValue;
	}
	
	EndContent(pCtxt);
}


//
// Gets page for the service requested - Expert, Logoff, etc.
//
void CIsapiServerExtension::GetPage(CHttpServerContext * pCtxt, LPSTR pstrClientID)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << "<P><FORM action=\"/cgi-bin/IsapiServer.dll?Header\" method=\"POST\" TARGET=\"Main\">\n";

	// Add variable for IP address here (replace hard coded 57.192.50.193) - JCD 
	// Add variable for IP address here (replace hard coded 57.192.50.193) - JCD 
	// Add variable for IP address here (replace hard coded 57.192.50.193) - JCD 

	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"F\" SRC=\"http://57.192.50.193/fares.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"R\" SRC=\"http://57.192.50.193/Rules.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"N\" SRC=\"http://57.192.50.193/footnotes.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"U\" SRC=\"http://57.192.50.193/Routings.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"A\" SRC=\"http://57.192.50.193/Addons.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"G\" SRC=\"http://57.192.50.193/Globals.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"Z\" SRC=\"http://57.192.50.193/Zones.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"E\" SRC=\"http://57.192.50.193/Expert.gif\">\n";

	*pCtxt << "<p><img src=\"http://57.192.50.193/Sita42.gif\" width=\"165\" height=\"43\">\n";
	*pCtxt << "<font color=\"#000080\" size=\"6\" face=\"Arial\">Fare Services - </font>\n";
	*pCtxt << "<font color=\"#FF0080\" size=\"6\"><em>winfare</em></font></p>\n";

	*pCtxt << "<INPUT TYPE=\"image\" NAME=\"L\" SRC=\"http://57.192.50.193/Logoff.gif\">\n";
	*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"ClientID\" VALUE=\"" << pstrClientID << "\">\n";
	*pCtxt << "</FORM>";

	EndContent(pCtxt);
}


void CIsapiServerExtension::Expert(CHttpServerContext *pCtxt, LPTSTR pstrCommand, LPSTR pstrClientID)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << "<H2><CENTER>EXPERT</CENTER></H2>";

	ExpertRequest er;
	strcpy(er.szTransaction, pstrCommand);

	if (ProcessRequest(EXPERT_REQUEST, pstrClientID, &er)==NO_WAFER_ERROR)
		*pCtxt << "<H2><CENTER>NO_WAFER_ERROR for Expert mode.</CENTER></H2>";
	else
		*pCtxt << "<H2><CENTER>Expert unsuccessful.</CENTER></H2>";


	EndContent(pCtxt);
}

void CIsapiServerExtension::Logout(CHttpServerContext * pCtxt, LPSTR pstrClientID)
{
	char cNull = NULL;

	StartContent(pCtxt);
	WriteTitle(pCtxt);
	
	if (ProcessRequest(LOGOFF_REQUEST, pstrClientID, &cNull)==NO_WAFER_ERROR)
		*pCtxt << "<H2><CENTER>You have successfully logged off the Wafer server.</CENTER></H2>";
	else
		*pCtxt << "<H2><CENTER>Unable to successfully log off from the Wafer server.</CENTER></H2>";

	EndContent(pCtxt);
}

/*
void CIsapiServerExtension::ProcessChildTermination(WPARAM w, LPARAM l)
{
	CString csOut;
	int nExitCode = (int)w;
	unsigned long ulThreadID = (unsigned long)l;

	TRACE("CIsapiServerExtension::ProcessChildTermination() called here ...\n");
	
//	if (nExitCode == 0) {
//		m_pClientManager->SetCompleted(ulThreadID, TRUE);
//	}
//	else {
//		m_pClientManager->SetCompleted(ulThreadID, FALSE);
//	}
	
}
*/

void CIsapiServerExtension::WriteTitle(CHttpServerContext* pCtxt) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHttpServer::WriteTitle(pCtxt);
}

void CIsapiServerExtension::Admin(CHttpServerContext * pCtxt, LPSTR pstrVar, LPSTR pstrAdminPW, LPSTR pstrAdminType)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << "<H1><CENTER>Wafer Server Administration Page</CENTER></H1><BR>\r\n";
	*pCtxt << "<hr color=\"#000080\">\r\n";

	ShowINIValues(pCtxt);

	*pCtxt << "<P><H3>\r\nAdmin Variables:<BR>";
	*pCtxt << "pstrVar      : " << pstrVar << "<BR>";
	*pCtxt << "pstrAdminPW  : " << pstrAdminPW << "<BR>";
	*pCtxt << "pstrAdminType: " << pstrAdminType << "</P>";

	switch(pstrAdminType[0])
	{	case 'L':
			LoginRequestData stLoginRequest;		// JCD - Add a LOGOFF type ?
	
			if (ProcessRequest(LOGOFF_REQUEST, pstrVar, &stLoginRequest)==NO_WAFER_ERROR)
				*pCtxt << "ClientID : " << pstrVar << "was logged off by Administration<BR>\r\n";
			else
				*pCtxt << "ClientID : " << pstrVar << "was NOT logged off<BR>\r\n";
			break;

		case 'E':
			m_bDeleteInactiveClients = TRUE;
			break;

		case 'D':
			m_bDeleteInactiveClients = FALSE;
			break;

		default:
		;
	}
	
	m_nNumberOfClients = m_pClientManager->GetCount();

	CString cs;
	cs.Format("There are %ld Child processes created<BR>\r\n", m_nChildProcesses);
	*pCtxt << cs;

	cs.Format("There are %ld clients logged on<BR>\r\n", m_nNumberOfClients );

	if (m_nNumberOfClients > 0)
	{	*pCtxt << cs;
		*pCtxt << "<P><FORM action=\"/cgi-bin/IsapiServer.dll?Admin\" method=\"POST\">\r\n";

    	*pCtxt << "Select Wafer Client to Logoff:";
    	*pCtxt << "<SELECT NAME=\"ClientID\" SIZE=\"1\">";
		*pCtxt << "<OPTION>ALL Clients</OPTION>";

		char szID[13];
		for (int n=0; n < m_nNumberOfClients; n++)
		{	m_pClientManager->GetClientIDByIndex(n, szID);

			*pCtxt << "<OPTION>" << szID << "</OPTION>";
		}
	
		*pCtxt << "</SELECT>&nbsp;<BR>\r\n";
		*pCtxt << "Admin Password: <INPUT TYPE=\"PASSWORD\" NAME=\"AdminPW\">\r\n";
		*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"AdminType\" VALUE=\"LOGOFF\">\r\n";
		*pCtxt << "<INPUT TYPE=\"SUBMIT\" VALUE=\"Logoff Selected Client\">\r\n";
		*pCtxt << "</FORM></P>";
	}

	if (m_bDeleteInactiveClients)
	{	*pCtxt << "<P><FORM action=\"/cgi-bin/IsapiServer.dll?Admin\" method=\"POST\">\r\n";
		*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"AdminType\" VALUE=\"DISABLE_CLEANER\">\r\n";
		*pCtxt << "<INPUT TYPE=\"SUBMIT\" VALUE=\"Disable Cleaner\">\r\n";
		*pCtxt << "</FORM></P>";
	}
	else
	{	*pCtxt << "<P><FORM action=\"/cgi-bin/IsapiServer.dll?Admin\" method=\"POST\">\r\n";
		*pCtxt << "<INPUT TYPE=\"HIDDEN\" NAME=\"AdminType\" VALUE=\"ENABLE_CLEANER\">\r\n";
		*pCtxt << "<INPUT TYPE=\"SUBMIT\" VALUE=\"Enable Cleaner\">\r\n";
		*pCtxt << "</FORM></P>";
	}


	*pCtxt << "</H3>\r\n";
	EndContent(pCtxt);
}

DWORD CIsapiServerExtension::HttpExtensionProc(EXTENSION_CONTROL_BLOCK *pECB) 
{
	// TODO: Add your specialized code here and/or call the base class

	TRACE("\nCIsapiServerExtension::HttpExtensionProc called ...\n");
	
	return CHttpServer::HttpExtensionProc(pECB);
}

int CIsapiServerExtension::CallFunction(CHttpServerContext* pCtxt, LPTSTR pszQuery, LPTSTR pszCommand) 
{
	// TODO: Add your specialized code here and/or call the base class

	//TRACE("QUERY: %s COMMAND %s\n", pszQuery, pszCommand);
	
	return CHttpServer::CallFunction(pCtxt, pszQuery, pszCommand);
}

void CIsapiServerExtension::ShowINIValues(CHttpServerContext * pCtxt)
{
	/***************************************************************/
	/* DO NOT add StartContent() / EndContent() to this function ! */
	/***************************************************************/

	*pCtxt << "<H3>\r\n";

	if (m_bInitOK)
		*pCtxt << "Wafer Server Initialized Successfully\r\n";
	else
		*pCtxt << "Wafer Server WAS NOT INITIALIZED\r\n";
	
	CString cs1, cs2, cs3, cs4, cs5;
	cs1.Format("%d", m_nAppListeningPort);
	cs2.Format("%d", m_nStartPID);
	cs3.Format("%d", m_nStopPID);
	cs4.Format("%d %s", m_nInactivityTimer, m_csInactivityTimerUnits);
	cs5.Format("%d", m_nDCPPortNumber);

	*pCtxt << "ISAPIServer Settings from INI file:<PRE>";
	*pCtxt << "Application Listening Port  :" << cs1 << "<BR>";
	*pCtxt << "Start PID                   :" << cs2 << "<BR>";
	*pCtxt << "Stop PID                    :" << cs3 << "<BR>";
	*pCtxt << "Inactivity Timer            :" << cs4 << "<BR>";
	*pCtxt << "DCP IP Address              :" << m_csDCPIPAddress << "<BR>";
	*pCtxt << "DCP Port Number             :" << cs5 << "<BR>";
	*pCtxt << "HTML Templates Directory    :" << m_csHTMLTemplateDir << "<BR>";
	
	cs4.Format("Enable Cleaner Thread       :%d<BR>", m_bDeleteInactiveClients);
	cs5.Format("Cleaner Sleep Interval (sec):%d", m_nSecondsToSleep);

	*pCtxt << cs4 << cs5;
	*pCtxt << "</PRE>\r\n";

	cs1.Format("Wafer Server has been run %d times<BR>\r\n", m_nTimesRun);
	*pCtxt << cs1 << "Last Compiled: "<< __TIMESTAMP__;

	*pCtxt << "</H3>\r\n";
}

// ----------------------------------------------------------------------------
// Function name	: WaferServer::ProcessRequest
// Description	    : This is the routine that the MainThread/ParentProcess of
//                    Wafer server loops into. It does not do anything when the
//                    are no requests coming into the server.
// Return type		: BOOL 
// ----------------------------------------------------------------------------
WaferError CIsapiServerExtension::ProcessRequest(WIP_RequestType eRT, char* pszClientID, void* pvParams)
{
	WaferClient* pWaferClient;

	TRACE("CIsapiServerExtension::ProcessRequest(%d, %s, pvParams)\n", eRT, pszClientID); 

	if (eRT == LOGIN_REQUEST)
	{
		pWaferClient = m_pClientManager->CreateClient(eRT);

		if (pWaferClient == NULL)
		{
			return SESSION_UNAVAILABLE;
		}
		strcpy(pszClientID, pWaferClient->GetClientID());
	}
	else
	{
		pWaferClient = m_pClientManager->VerifyClient(pszClientID);

		if (pWaferClient == NULL)
		{
			return INVALID_CLIENT_ID;
		}
		else
		{
			pWaferClient->SetProperties(eRT);
		}
	}

	// Create & kill WaferChild for EVERY request.
	if (BeginWaferChild(pWaferClient, pvParams) == FALSE)
	{
		m_pClientManager->EndClient(pszClientID);
		TRACE("WaferServer::ProcessRequest - call to BeginWaferChild()==FALSE\n");
		return FORK_FAILED;
	}

	return NO_WAFER_ERROR;
}


// ----------------------------------------------------------------------------
// Function name	: WaferServer::BeginWaferChild
// Description	    : This function is responsible for starting up a WaferChild
//                    When used in a windows environment, "CreateThread" is
//                    called.
//                    In a Unix environment, "fork" is called.
// Return type		: BOOL - TRUE if successful on starting a WaferChild,
//                           FALSE otherwise.
// Argument         : WaferClient* pWaferClient - WaferClient is the object used
//                    by WaferChild for processing client requests.
// ----------------------------------------------------------------------------
BOOL CIsapiServerExtension::BeginWaferChild(WaferClient* pWaferClient, void* pvParams)
{
	TRACE("CIsapiServerExtension::BeginWaferChild\n"); 

	CWaferChild* pWaferChild = new CWaferChild(pWaferClient, pvParams);
	m_nChildProcesses++;

	m_pClientManager->SetClientActive(pWaferClient, 0, TRUE);

	int nTransactionResult = pWaferChild->Process();

	if (nTransactionResult != 0)
	{
		m_pClientManager->EndClient(pWaferClient->GetClientID());
		m_nChildProcesses--;
	}
	else {
		m_pClientManager->SetClientActive(pWaferClient, 0, FALSE);
	}
	return TRUE;

}

UINT CIsapiServerExtension::CleanerFunction(void * pvParms)
{
	CIsapiServerExtension* pThis = (CIsapiServerExtension*) pvParms;

	while (pThis->m_bDeleteInactiveClients)
	{	
		TRACE("Would delete inactive clients in this thread\r\n");
		Sleep(pThis->m_nSecondsToSleep * 1000);
	}
	return 0;
}

void CIsapiServerExtension::Test(CHttpServerContext * pCtxt, LPSTR E1, LPSTR E2, LPSTR E3, LPSTR E4, LPSTR E5, LPSTR E6, LPSTR E7, LPSTR E8, LPSTR E9, LPSTR E10, LPSTR 
							E11, LPSTR E12, LPSTR E13, LPSTR E14, LPSTR E15, LPSTR E16, LPSTR E17, LPSTR E18, LPSTR E19, LPSTR E20, LPSTR 
							E21, LPSTR E22, LPSTR E23, LPSTR E24, LPSTR E25, LPSTR E26, LPSTR E27, LPSTR E28, LPSTR E29, LPSTR E30, LPSTR E31)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	TRACE("Test function for debugging only\n");
	*pCtxt << "Test parms<BR>";
	*pCtxt << E1<< "<BR>" << E2<< "<BR>" << E3<< "<BR>" << E4<< "<BR>" << E5<< "<BR>" << E6<< "<BR>" << E7<< "<BR>" << E8<< "<BR>" << E9<< "<BR>" << E10<< "<BR>" << 
							E11<< "<BR>" << E12<< "<BR>" << E13<< "<BR>" << E14<< "<BR>" << E15<< "<BR>" << E16<< "<BR>" << E17<< "<BR>" << E18<< "<BR>" << E19<< "<BR>" << E20<< "<BR>" << 
							E21<< "<BR>" << E22<< "<BR>" << E23<< "<BR>" << E24<< "<BR>" << E25<< "<BR>" << E26<< "<BR>" << E27<< "<BR>" << E28<< "<BR>" << E29<< "<BR>" << E30<< "<BR>" << E31;
	EndContent(pCtxt);
}

BOOL CIsapiServerExtension::OnParseError(CHttpServerContext* pCtxt, int nCause) 
{
	TRACE("OnParseError nCause = %d\n", nCause);
	
	return CHttpServer::OnParseError(pCtxt, nCause);
}
