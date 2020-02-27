#ifndef WAFER_INCLUDED
#define WAFER_INCLUDED

#include "Defs.h"
#include "Errors.h"	// Wafer Error Architecture

enum WIP_RequestType 
{
	LOGIN_REQUEST=1,
	LOGOFF_REQUEST,
	FARES_REQUEST,
	RULES_REQUEST,
	PARAGRAPHS_REQUEST,
	PARAGRAPHTEXT_REQUEST,
	FOOTNOTES_REQUEST,
	FOOTNOTEITEMS_REQUEST,
	EXPERT_REQUEST
};

enum WIP_ResponseType
{
	WIP_ERROR=0,
	LOGIN_RESPONSE,
	FARES_RESPONSE,
	EXPERT_RESPONSE,
	LOGOFF_RESPONSE,
	UNKNOWN_CONDITION
};

enum WIP_ClientType {
	USER_INTERFACE=0,
	CGI_PROGRAM
};

struct WIP_RequestHeader {
	char			szClientID[MAX_CLIENTID_LEN+1];
	WIP_RequestType	eRequestType;
	WIP_ClientType	eClientType;
};

struct WIP_ResponseHeader {
	WIP_ResponseType	eResponseType;
	char				szResponseTime[10]; // Purely for debugging purposes.
};

struct LoginRequestData {
	char szUserId[MAX_USERID_LEN+1];
	char szPassword[MAX_PASSWORD_LEN+1];
	char szSystem[MAX_SYSTEM_LEN+1];
	char szIPAddress[MAX_IPADDR_LEN+1];
};

struct LoginResponseData {
	char szClientID[13];
};

// The following 7 structures are not yet a standard. For test purposes only
struct FaresRequestData {
	char szOrigin[MAX_ORIGIN_LEN+1];
	char szDestn[MAX_DESTN_LEN+1];
	char szCarrier[MAX_CARRIER_LEN+1];
};

struct FaresResponseHeader {
	long nFares;
};

struct FaresResponseData {
	char *szBuffer;
};

struct ExpertRequest {
	char szTransaction[MAX_TRANSACTION_LEN+1];
};

struct ExpertResponseHeader {
	long nRespLen;
};

struct ExpertResponseData {
	char *szBuffer;
};

#endif
