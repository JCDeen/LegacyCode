/////////////////////////////////////////////////////////////////////////////
//																		   //
// Implementation of WAFER Document T0021: Error Messaging Architecture.   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

#ifndef ERRORS_INCLUDED
#define ERRORS_INCLUDED

#define	LOGIN_FAILED			1
#define	SEND_ERROR				2
#define	RECEIVE_ERROR			4
#define	MAINFRAME_TIMEOUT		8
#define	REQUEST_TIMEOUT			16
#define	LOGGED_OUT				32
#define BAD_REQUEST_TYPE		64
#define CHANGE_SYSTEM_FAILED	128
#define TRANSACTION_FAILED		256

enum WaferError {
	NO_WAFER_ERROR=0,
	FORK_FAILED,
	INVALID_CLIENT_ID,
	SESSION_UNAVAILABLE
};

enum ErrorLevel {
	MESSAGE=1,	// Message for information purposes only
	DEBUG,		// Debug messages
	WARNING,	// Indication of some error, but not significant
	SIGNIFICANT,// Indication of real significant error
	FATAL		// Indication of very severe error
};

struct ErrorResponseData {
	short		ErrorID;
	ErrorLevel	eLevel;
	char		szMessage[40];
};


#endif
