// WaferChild.cpp : implementation file
//
#include <time.h>
#include <sys/timeb.h>
#include <string>
#include <iostream>
#include <fstream>

#include "stdafx.h"
#include "WaferClient.h"
#include "MFTransaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


WaferClient::WaferClient(WIP_RequestType eRT, void* pvParams)
{
	m_pvParams = pvParams;
	m_pInterface = new MFSockInterface;
	m_eRequestType = eRT;
}

/*
WaferClient::WaferClient(WaferClient* pWaferClient, void* pvParams)
{
	TRACE(" WaferClient::WaferClient\n"); 

	m_pInterface = pWaferClient->GetInterface();
	m_eRequestType = (WIP_RequestType)pWaferClient->GetRequestType();
	//m_szClientID = pWaferClient->GetClientID();
	m_ExitStatus = 0;
}
*****/

WaferClient::~WaferClient()
{
	delete m_pInterface;
}

void WaferClient::SetProperties(WIP_RequestType eRT)
{
	m_eRequestType = eRT;
}

BOOL WaferClient::Initialize(void)
{
	return m_pInterface->Initialize(gpstWaferConfig);
}


WIP_RequestType		WaferClient::GetRequestType(void)
{ 
	return m_eRequestType; 
}

MFSockInterface*	WaferClient::GetInterface(void)
{	
	return m_pInterface; 
}

char* WaferClient::GetClientID(void) 
{ 
	return m_szClientID; 
}

void WaferClient::Terminate(void)
{
	m_pInterface->Disconnect();
}

void WaferClient::SetClientID(char* szID)
{
	strcpy(m_szClientID, szID);
}

void WaferClient::SetActive(BOOL bActive = TRUE)
{
	m_bActive = bActive;
}
	
void WaferClient::SetInActive()
{
	m_bActive = FALSE;
}
	
BOOL WaferClient::IsActive(void)
{
		return (m_bActive);
}

int WaferClient::Process()
{
	TRACE( " WaferClient::Process\n"); 

	switch(m_eRequestType) {
		case LOGIN_REQUEST:
			ProcessLogin();
			break;

		case EXPERT_REQUEST:
			ProcessExpert();
			break;

		case LOGOFF_REQUEST:
			ProcessLogout();
			break;

		default:
			SetError(BAD_REQUEST_TYPE);
			break;
	}
	return m_ExitStatus;
}

void WaferClient::ProcessLogin()
{
	BOOL bResult;
	MFTransaction* pTransaction = new MFTransaction(m_pInterface);

	TRACE( " WaferClient::ProcessLogin\n"); 

	bResult = pTransaction->Login((LoginRequestData *)m_pvParams);

	if (bResult == FALSE) {
		SetError(LOGIN_FAILED);
	}
	else {
		bResult = pTransaction->ChangeSystem(((LoginRequestData *)m_pvParams)->szSystem);

		if (bResult == FALSE) {
			SetError(CHANGE_SYSTEM_FAILED);
		}
	}
	delete pTransaction;
}

void WaferClient::ProcessExpert()
{
	//char*			szResult;

	TRACE( " WaferClient::ProcessExpert\n"); 

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);
		
	if (pTransaction->Expert((ExpertRequest *)m_pvParams) != NULL)
	{	
		m_szResults = (char *)calloc(pTransaction->m_lMFOutSize+1, sizeof(char));
		strcpy(m_szResults, pTransaction->GetMFOut());
	}
	else
	{
		SetError(TRANSACTION_FAILED);
	}

	delete pTransaction;
}

void WaferClient::ProcessLogout()
{
	TRACE(" WaferClient::ProcessLogout\n"); 

	MFTransaction* pTransaction = new MFTransaction(m_pInterface);
	pTransaction->Logout();
	
	SetError(LOGGED_OUT);

	delete pTransaction;
}

void WaferClient::SetError(int nError)
{
	TRACE(" WaferClient::SetError\n"); 

	m_ExitStatus |= nError;
}
