/////////////////////////////////////////////////////////////////
// ParsingClass.cpp: implementation of the ParsingClass class  //
// Documentation source :									   //
// Author: James Christian Deen								   //
/////////////////////////////////////////////////////////////////

#include "ParsingClass.h"
//////////////////////////////////////////////////////////////////////
//						Construction/Destruction					//
//////////////////////////////////////////////////////////////////////

ParsingClass::ParsingClass()
{
	m_bEnableTrace = false;

	memset(m_szDataBuffer, '\0', ACTUAL_BUFFER_SIZE);
	sprintf(m_szSafe, "%c", THE_SAFE_CHAR);

	m_eErrorStatus = EVERYTHING_IS_FINE;
	m_bError = false;
	m_nDebug = 0;

}

ParsingClass::~ParsingClass()
{

//	if (m_pszFromMainframe != NULL)
//		free(m_pszFromMainframe);	// Free up previously allocated memory.
}


////////////////////////////////////////////////////////
// Ensures that last error status is not overwritten. //
////////////////////////////////////////////////////////
void ParsingClass::SetErrorStatus(int eStatusCode)
{	if (m_eErrorStatus == EVERYTHING_IS_FINE)
		m_eErrorStatus = eStatusCode;
}


char *ParsingClass::PointToNextField(const char* szToBeSearched, char *szDelimiter, bool bReplaceDelimiter)
{	char *ptr;
	ptr = strstr(szToBeSearched, szDelimiter);

	if (ptr != NULL)
	{	if (bReplaceDelimiter)
			*(ptr) = EOT;
		m_bError = false;
		return (ptr + strlen(szDelimiter));
	}
	else
	{	m_bError = true;
		SetErrorStatus(DELIMITER_NOT_FOUND);
		return (m_szSafe);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Use this version of function if szToBeSearched may contain the delimiter as data. //
//		szToBeSearched:															     //
//			ptr to string position to begin searching in.							 //
//		nNextDataOffset:									                         //
//			Character position where next field of data starts @ 1 past delimiter.   //
///////////////////////////////////////////////////////////////////////////////////////
char *ParsingClass::PointToNextField(const char* szToBeSearched, int nNextDataOffset, bool bReplaceDelimiter)
{	char *ptr;

	ptr = (char *)szToBeSearched + nNextDataOffset;

	if (bReplaceDelimiter)
		*(ptr-1) = EOT;

	m_bError = false;
	SetErrorStatus(INTERNAL_TRACE1);

	return (ptr);
}

///////////////////////////////////////////////
// Finds next occurence of "\r\n" within the //
// null terminated string being searched.    //
///////////////////////////////////////////////
char *ParsingClass::PointToNextLine(const char* szToBeSearched)
{	char *ptr;
	ptr = strstr(szToBeSearched, "\r\n");

	m_bError = false;
	SetErrorStatus(INTERNAL_TRACE2);

	if (ptr != NULL)
		return (ptr + 2);
	else
		return (m_szSafe);
}

char *ParsingClass::ReturnErrorString()
{	switch (m_eErrorStatus)
	{	case EVERYTHING_IS_FINE:
			return "No Wafer Errors Detected";
		case NO_ETX:
			return "NO_ETX";
		case FIELD_ERROR:
			return "FIELD_ERROR";
		case DELIMITER_NOT_FOUND:
			return "DELIMITER_NOT_FOUND";
		case NO_TRANSACTION_STRING:
			return "NO_TRANSACTION_STRING";
		case SEND_LESS_THAN_ZERO:
			return "SEND_LESS_THAN_ZERO";

		case NCHARS_MORE_THANBUFFER_SIZE:
			return "NCHARS_MORE_THANBUFFER_SIZE";

		case INTERNAL_TRACE1:
			return "INTERNAL_TRACE 1";
		case INTERNAL_TRACE2:
			return "INTERNAL_TRACE 2";
		case INTERNAL_TRACE3:
			return "INTERNAL_TRACE 3";
		case INTERNAL_TRACE4:
			return "INTERNAL_TRACE 4";

		case DERIVED_CLASS_ERROR1:
			return "Derived Class Error 1";
		case DERIVED_CLASS_ERROR2:
			return "Derived Class Error 2";
		case DERIVED_CLASS_ERROR3:
			return "Derived Class Error 3";
		case DERIVED_CLASS_ERROR4:
			return "Derived Class Error 4";

		default:
			return "DEFAULT ERROR TRAP";
	}
}


