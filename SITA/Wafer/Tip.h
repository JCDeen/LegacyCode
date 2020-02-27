#ifndef TRANSACTION_INCLUDED
#define TRANSACTION_INCLUDED

#include "Defs.h"
#include "Errors.h"

/////////////////////////////////////////////////////////////////////////////
//																		   //
// Implementation of WAFER Document T00020: WAFER Interface Specifications //
//																		   //
/////////////////////////////////////////////////////////////////////////////

enum TIP_RequestType 
{
	SESSION_ESTABLISH=1,
	SESSION_CLOSE,
	TRANSACTION_REQUEST
};

enum TIP_ResponseType
{
	TIP_ERROR=0,
	TSESSION_ESTABLISHED,
	SESSION_CLOSED,
	TRANSACTION_RESPONSE
};

struct TIP_RequestHeader {
	TIP_RequestType	eRequestType;
	long			lSize;
};

struct TIP_ResponseHeader {
	TIP_ResponseType eResponseType;
	long			 lSize;
};

struct SessionEstablishData {
	char		szUserId[MAX_USERID_LEN+1];
	char		szPassword[MAX_PASSWORD_LEN+1];
	char		szSystem[MAX_SYSTEM_LEN+1];
};

struct SessionEstablishedData {
	char		szSessionID[13];
};

struct SessionCloseData {
	char		szSessionID[13];
};

// TRANSACTION_REQUEST_DATA		- variable length string - see WAFER Document: T0020
// TRANSACTION_RESPONSE_DATA	- variable length string - see WAFER Document: T0020

#endif
