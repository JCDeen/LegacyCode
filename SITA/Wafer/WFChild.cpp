#include <time.h>
#include <sys/timeb.h>
//#include <string>

#ifdef _WIN32

#include <iostream>
#include <fstream>

#else

#include <fstream.h>
#include <iostream.h>

#endif

using namespace std ;

#include "WFChild.hpp"
#include "MFTransaction.h"

// ----------------------------------------------------------------------------
// Function name	: WaferChild::WaferChild
// Description	    : Constructor for WaferChild. This would be a new thread or
//                    or a process.
// Return type		: None
// Argument         : WaferClient* pWaferClient - These are the properties
//                    associated with every client.
// ----------------------------------------------------------------------------
WaferChild::WaferChild(WaferClient* pWaferClient)
{
	cout << "Beginning WaferChild....\n";

	m_eClientType = (WIP_ClientType)pWaferClient->GetClientType();
	m_pSocket = pWaferClient->GetWaferSocket();
	m_pInterface = pWaferClient->GetTransaction();
	m_eRequestType = (WIP_RequestType)pWaferClient->GetRequestType();
	m_szClientID = pWaferClient->GetClientID();
	m_ExitStatus = 0;
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::Begin
// Description	    : Starts working on the new request. This loops forever if
//                    the client is a User Interface. If the client is a CGI
//                    program, the appropriate request gets processed and the
//                    WaferChild exits right away.
// Return type		: void 
// ----------------------------------------------------------------------------
void WaferChild::Begin()
{
	do {
		Process();

		if (m_eClientType == USER_INTERFACE) {
			WIP_RequestHeader	stRequestHeader;

			if (m_pSocket->Receive((char *)&stRequestHeader, sizeof(WIP_RequestHeader)) == FALSE) {
				perror("Receive Request Type");
				SetError(RECEIVE_ERROR);
				Exit();
			}
			m_eRequestType = (WIP_RequestType)ntohs(stRequestHeader.eRequestType);
		}

	} while (m_eClientType != CGI_PROGRAM);

	Exit();
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::Process
// Description	    : Process the appropriate request.
// Return type		: void 
// ----------------------------------------------------------------------------
void WaferChild::Process()
{
	switch(m_eRequestType) {
		case LOGIN_REQUEST:
			ProcessLogin();
			break;

		case FARES_REQUEST:
			ProcessFares();
			break;

		case EXPERT_REQUEST:
			ProcessExpert();
			break;

		case RULES_REQUEST:
			ProcessRules();
			break;

		case PARAGRAPHS_REQUEST:
			ProcessParagraphs();
			break;

		case FOOTNOTES_REQUEST:
			ProcessFootNotes();
			break;

		case LOGOFF_REQUEST:
			ProcessLogout();
			break;

		default:
			SetError(BAD_REQUEST_TYPE);
			break;
	}
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::ProcessLogin
// Description	    : Method to login to the mainframe or the transaction server,
//                    This method also invokes the ChangeSystem method.
// Return type		: void 
// Argument         : void
// ----------------------------------------------------------------------------
void WaferChild::ProcessLogin(void)
{
	WIP_ResponseHeader	stResponse;
	LoginRequestData	stLoginReq;
	LoginResponseData	stLoginResp;
	BOOL				bResult;
	struct timeb		stStartTime, stEndTime;

	if (m_pSocket->Receive((char *)&stLoginReq, sizeof(stLoginReq)) == FALSE) {
		perror("Receive Login Request");
		SetError(RECEIVE_ERROR);
		return;
	}

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);

	ftime(&stStartTime);
	bResult = pTransaction->Login(&stLoginReq);

	if (bResult == FALSE) {
		// Send the WIP_ResponseHeader
		stResponse.eResponseType = (WIP_ResponseType)htons(WIP_ERROR);
		m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

		// Send reason for the error
		ErrorResponseData	stErrorResponse;
		
		stErrorResponse.ErrorID = LOGIN_FAILED;
		stErrorResponse.eLevel = FATAL;
		strcpy(stErrorResponse.szMessage, "Invalid UserID/Password");

		m_pSocket->Send((char *)&stErrorResponse, sizeof(ErrorResponseData));
		
		SetError(LOGIN_FAILED);
	}
	else {
		bResult = pTransaction->ChangeSystem(stLoginReq.szSystem);

		if (bResult == FALSE) {
			stResponse.eResponseType = (WIP_ResponseType)htons(WIP_ERROR);
			m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

			// Send reason for the error
			ErrorResponseData	stErrorResponse;
		
			stErrorResponse.ErrorID = LOGIN_FAILED;
			stErrorResponse.eLevel = FATAL;
			strcpy(stErrorResponse.szMessage, "Change System failed");

			m_pSocket->Send((char *)&stErrorResponse, sizeof(ErrorResponseData));
					
			SetError(LOGIN_FAILED);
		}
		else {
			ftime(&stEndTime);
			sprintf(stResponse.szResponseTime, "%05.3f", (float)(stEndTime.time - stStartTime.time) + (float)(stEndTime.millitm - stStartTime.millitm) / 1000);
			stResponse.eResponseType = (WIP_ResponseType)htons(LOGIN_RESPONSE);

			m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

			strncpy(stLoginResp.szClientID, m_szClientID.data(), MAX_CLIENTID_LEN);
			stLoginResp.szClientID[MAX_CLIENTID_LEN] = '\0';

			m_pSocket->Send((char *)&stLoginResp, sizeof(LoginResponseData));
		}
	}
	delete pTransaction;
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::ProcessExpert
// Description	    : Process Expert transaction.
// Return type		: void 
// Argument         : void
// ----------------------------------------------------------------------------
void WaferChild::ProcessExpert(void)
{
	WIP_ResponseHeader		stResponse;
	ExpertRequest			stExpertReq;
	ExpertResponseHeader	stExpertRespHeader;
	char					*szResult;
	struct timeb			stStartTime, stEndTime;

	ofstream StatFile("WFServer.DAT", ios::out | ios::app);

	if (m_pSocket->Receive((char *)&stExpertReq, sizeof(stExpertReq)) == FALSE) {
		perror("Receive Expert Request");
		SetError(RECEIVE_ERROR);
		return;
	}

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);
	ftime(&stStartTime);
	szResult = pTransaction->Expert(&stExpertReq);

	if (szResult == NULL) {
		// Send the WIP_ResponseHeader
		stResponse.eResponseType = (WIP_ResponseType)htons(WIP_ERROR);
		m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

		// Send reason for the error
		ErrorResponseData	stErrorResponse;
		
		stErrorResponse.ErrorID = MAINFRAME_TIMEOUT;
		stErrorResponse.eLevel = WARNING;
		strcpy(stErrorResponse.szMessage, "Mainframe did not respond to the command");

		m_pSocket->Send((char *)&stErrorResponse, sizeof(ErrorResponseData));
	}
	else {
		ftime(&stEndTime);
		sprintf(stResponse.szResponseTime, "%05.3f", (float)(stEndTime.time - stStartTime.time) + (float)(stEndTime.millitm - stStartTime.millitm) / 1000);
//		StatFile << "Transaction <" << (string)stExpertReq.szTransaction << "> Time Taken = <" << stResponse.szResponseTime << ">\n";
		StatFile.close();

		stResponse.eResponseType = (WIP_ResponseType)htons(EXPERT_RESPONSE);

		m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

		stExpertRespHeader.nRespLen = htonl(strlen(szResult));
		m_pSocket->Send((char *)&stExpertRespHeader, sizeof(ExpertResponseHeader));

		m_pSocket->Send(szResult, strlen(szResult));
	}
	delete pTransaction;
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::ProcessLogout
// Description	    : Logout from the mainframe.
// Return type		: void 
// Argument         : void
// ----------------------------------------------------------------------------
void WaferChild::ProcessLogout(void)
{
	WIP_ResponseHeader	stResponse;

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);

	pTransaction->Logout();
	
	stResponse.eResponseType = (WIP_ResponseType)htons(LOGOFF_RESPONSE);
	m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

	SetError(LOGGED_OUT);

	delete pTransaction;
	Exit();
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::Exit
// Description	    : This call would end the thread/process. The private member
//                    variable 'm_ExitStatus' indicates the result of the
//                    transactions between WaferChild and the Mainframe /
//                    Transaction server.
// Return type		: void 
// ----------------------------------------------------------------------------
void WaferChild::Exit()
{
	WaferExit(m_ExitStatus);
}


// ----------------------------------------------------------------------------
// Function name	: WaferChild::ProcessFares
// Description	    : Not implemented yet.
// Return type		: void 
// ----------------------------------------------------------------------------
void WaferChild::ProcessFares()
{
	FaresRequestData	stFaresReq;
	FaresResponseHeader	stFaresResponseHeader;
	WIP_ResponseHeader	stResponse;

	if (m_pSocket->Receive((char *)&stFaresReq, sizeof(stFaresReq)) == FALSE) {
		perror("Receive Fares Request");
		SetError(RECEIVE_ERROR);
		return;
	}

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);

	char* szResult = pTransaction->Fares(&stFaresReq);

	stResponse.eResponseType = (WIP_ResponseType)htons(FARES_RESPONSE);
	m_pSocket->Send((char *)&stResponse, sizeof(WIP_ResponseHeader));

	stFaresResponseHeader.nFares = htonl(strlen(szResult));
	m_pSocket->Send((char *)&stFaresResponseHeader, sizeof(FaresResponseHeader));

	m_pSocket->Send(szResult, strlen(szResult));

	delete pTransaction;
}

