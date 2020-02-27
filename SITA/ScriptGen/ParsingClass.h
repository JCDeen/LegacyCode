// ParsingClass.h: interface for the ParsingClass class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ParsingClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_)
#define AFX_ParsingClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_

#include <stdio.h>		// Move to an include file
#include <string.h>
#include <iostream.h>
#include <time.h>
#include <fstream.h>

#pragma message ("ParsingClass.h included ....")

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define Windows 1       // Windows
//#define Windows 0     // Unix

#define USABLE_BUFFER_SIZE 10000
#define ACTUAL_BUFFER_SIZE 10002

#define MAX_LINES_PER_PAGE        25	
#define MAX_COLUMNS_PER_PAGE      80
#define MAX_PAGES_PER_TRANSACTION 63

#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04
#define ENQ 0x05
#define ACK 0x06
#define BEL 0x07
#define THE_SAFE_CHAR '?'
#define DELIMITER_REPLACEMENT_FLAG EOT

class ParsingClass 
{
public:
	ParsingClass();												// Use for Rendezvous access.
	virtual ~ParsingClass();

	int m_nCommandLineDebugMode;
	bool m_bEnableTrace;

	char m_szDataBuffer[ACTUAL_BUFFER_SIZE];  // I/O buffer - holds data to be parsed
	char *m_pPageCurrent, *m_pPageTotal, *m_pDisplayOrTicket, *m_pErrorString;
	char *m_pMainframeStatusText;

	int m_eErrorStatus; // Probably make this of enum type below ...

	enum myErrorCodes {EVERYTHING_IS_FINE=0, NO_ETX, FIELD_ERROR, DELIMITER_NOT_FOUND,
					  NO_TRANSACTION_STRING, SEND_LESS_THAN_ZERO, ERROR_IN_CONNECT, 
					  NCHARS_MORE_THANBUFFER_SIZE, INTERNAL_TRACE1, INTERNAL_TRACE2,
					  INTERNAL_TRACE3, INTERNAL_TRACE4, DERIVED_CLASS_ERROR1,
					  DERIVED_CLASS_ERROR2,	DERIVED_CLASS_ERROR3, DERIVED_CLASS_ERROR4 };

	///////////////////////////////////////////////////////////////////////
	// DERIVED_CLASS_ERROR1 thru DERIVED_CLASS_ERROR4 are to be used in  //
	// classes derived from this class to report error conditions as the //
	// programmer sees fit.												 //
	///////////////////////////////////////////////////////////////////////
protected:
	bool m_bError;	// Status of latest operation.
	int m_nDebug;

private:
	char m_szSafe[2];		// "Safe" vector for initializing pointers so != NULL.

public:
	//bool ReadConfigurationFile(char *szConfigFileName);

	/////////////////
	// Operations. //
	/////////////////
	char *ReturnErrorString();
	void SetErrorStatus(int eStatusCode);

	void SetSafeArrayCharacter(char c)	{sprintf(m_szSafe, "%c", c); }
	void RestoreSafeArrayCharacter()	{sprintf(m_szSafe, "%c", THE_SAFE_CHAR); }
	char *ReturnSafeArray()				{return m_szSafe; }

	bool ReturnM_bError()			{return m_bError; }

	char *PointToNextField(const char* szToBeSearched, char *szDelimiter, bool bReplaceDelimiter = true);
	char *PointToNextField(const char* szToBeSearched, int nOffset, bool bReplaceDelimiter = true);
	char *PointToNextLine(const char* szToBeSearched);

};

#endif // !defined(AFX_ParsingClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_)
