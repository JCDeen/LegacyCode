#include <malloc.h>

#include "TIPTransaction.h"
#include "Config.h"

extern WaferConfig* gpstWaferConfig;


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::TIPTransaction
// Description	    : Constructor. Creates a socket and keeps it ready to
//                    communicate to the mainframe.
// Return type		: None
// ----------------------------------------------------------------------------
TIPTransaction::TIPTransaction()
{
	m_pTransSocket = new WaferClientSocket;
	m_bLoggedIn = FALSE;
}


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::~TIPTransaction
// Description	    : Destructor. Delete the socket to the transaction socket.
// Return type		: none
// ----------------------------------------------------------------------------
TIPTransaction::~TIPTransaction()
{
	delete m_pTransSocket;
}


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::Initialize
// Description	    : Initializes the socket for communication.
// Return type		: BOOL 
// ----------------------------------------------------------------------------
BOOL TIPTransaction::Initialize()
{	
	return m_pTransSocket->Create();
}


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::Login
// Description	    : Login to the mainframe..
// Return type		: BOOL - TRUE if successful, FALSE otherwise.
// Argument         : LoginRequestData* pstLoginRequest - The data that the
//                    external client sends as login information.
// ----------------------------------------------------------------------------
BOOL TIPTransaction::Login(LoginRequestData* pstLoginRequest)
{
	if (m_pTransSocket->ConnectToServer(gpstWaferConfig->szTransServerAddress, gpstWaferConfig->nTransServerPort) == FALSE) {
		return FALSE;
	}

	TIP_RequestHeader stTIPRequest;
	
	stTIPRequest.eRequestType = (TIP_RequestType) htons(SESSION_ESTABLISH);
	stTIPRequest.lSize = htonl(sizeof(SessionEstablishData));

	if (m_pTransSocket->Send((char *)&stTIPRequest, sizeof(TIP_RequestHeader)) == FALSE) {
		return FALSE;
	}

	SessionEstablishData stEstablish;
	
	strcpy(stEstablish.szUserId, pstLoginRequest->szUserId);
	strcpy(stEstablish.szPassword, pstLoginRequest->szPassword);
	strcpy(stEstablish.szSystem, pstLoginRequest->szSystem);
	
	if (m_pTransSocket->Send((char *)&stEstablish, sizeof(SessionEstablishData)) == FALSE) {
		return FALSE;
	}

	TIP_ResponseHeader stTIPResponse;
	
	if (m_pTransSocket->Receive((char *)&stTIPResponse, sizeof(stTIPResponse)) == FALSE) {
		return FALSE;
	}
	stTIPResponse.eResponseType = (TIP_ResponseType) ntohs(stTIPResponse.eResponseType);

	if (stTIPResponse.eResponseType == TSESSION_ESTABLISHED) {
		SessionEstablishedData stEstablished;
	
		if (m_pTransSocket->Receive((char *)&stEstablished, sizeof(SessionEstablishedData)) == FALSE) {
		}
	}
	else {
		ErrorResponseData stErrorResponse;
		
		m_pTransSocket->Receive((char *)&stErrorResponse, sizeof(ErrorResponseData));
		return FALSE;
	}

	m_bLoggedIn = TRUE;
	return TRUE;
}

char* TIPTransaction::Fares(FaresRequestData* pstFaresRequest)
{
	return NULL;
}


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::Expert
// Description	    : Expert transaction
// Return type		: char* - Results of the expert transaction.
// Argument         : ExpertRequest* pstExpertRequest
// ----------------------------------------------------------------------------
char* TIPTransaction::Expert(ExpertRequest* pstExpertRequest)
{
	TIP_RequestHeader stTIPRequest;
	
	stTIPRequest.eRequestType = (TIP_RequestType) htons(TRANSACTION_REQUEST);
	stTIPRequest.lSize = htonl(sizeof(ExpertRequest));

	if (m_pTransSocket->Send((char *)&stTIPRequest, sizeof(TIP_RequestHeader)) == FALSE) {
		return NULL;
	}

	if (m_pTransSocket->Send((char *)pstExpertRequest, sizeof(ExpertRequest)) == FALSE) {
		return NULL;
	}
	
	ExpertResponseHeader stExpertResponseHeader;
	if (m_pTransSocket->Receive((char *)&stExpertResponseHeader, sizeof(ExpertResponseHeader)) == FALSE) {
		return NULL;
	}
	stExpertResponseHeader.nRespLen = (long) ntohl(stExpertResponseHeader.nRespLen);
	
	char* pszBuffer = (char*)calloc(stExpertResponseHeader.nRespLen+1, sizeof(char));
	if (m_pTransSocket->Receive(pszBuffer, stExpertResponseHeader.nRespLen) == FALSE) {
		return NULL;
	}
	
	return pszBuffer;
}


// ----------------------------------------------------------------------------
// Function name	: TIPTransaction::Logout
// Description	    : Logout from the mainframe,
// Return type		: BOOL - TRUE if logged out succesfully, FALSE otherwise.
// ----------------------------------------------------------------------------
BOOL TIPTransaction::Logout()
{
	TIP_RequestHeader stTIPRequest;
	
	stTIPRequest.eRequestType = (TIP_RequestType) htons(SESSION_CLOSE);
	stTIPRequest.lSize = htonl(sizeof(SessionCloseData));

	if (m_pTransSocket->Send((char *)&stTIPRequest, sizeof(TIP_RequestHeader)) == FALSE) {
		return FALSE;
	}

	m_pTransSocket->Close();
	m_bLoggedIn = FALSE;
	return TRUE;
}

