// SegmentDataClass.cpp: implementation of the SegmentDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "SegmentDataClass.h"
#include <ctype.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SegmentDataClass::SegmentDataClass()
{
	InitMemberVariables();
}

SegmentDataClass::~SegmentDataClass()
{
}

void SegmentDataClass::InitMemberVariables (void)
{
	memset(m_szSegmentStatus,	'\0', sizeof(m_szSegmentStatus));
	memset(m_szCarrierCode,		'\0', sizeof(m_szCarrierCode));
	memset(m_szFlightNumber,	'\0', sizeof(m_szFlightNumber));
	memset(m_szRBD,				'\0', sizeof(m_szRBD));				// AKA "booking Code" ?
	memset(m_szDepartureDate,	'\0', sizeof(m_szDepartureDate));
	memset(m_szDeparturePoint,	'\0', sizeof(m_szDeparturePoint));
	memset(m_szDepartureTime,	'\0', sizeof(m_szDepartureTime));
	memset(m_szDateChangeIndicator, '\0', sizeof(m_szDateChangeIndicator));

	memset(m_szArrivalDate, '\0', sizeof(m_szArrivalDate));
	memset(m_szArrivalPoint,'\0', sizeof(m_szArrivalPoint));
	memset(m_szArrivalTime, '\0', sizeof(m_szArrivalTime));

	memset(m_szNumberOfStopovers, '\0', sizeof(m_szNumberOfStopovers));
	memset(m_szStopoverIndicator,'\0', sizeof(m_szStopoverIndicator));

	memset(m_szGlobalIndicator, '\0', sizeof(m_szGlobalIndicator));
	memset(m_szEquipmentType,	'\0', sizeof(m_szEquipmentType));
	memset(m_szFareConstructionIndicator, '\0', sizeof(m_szFareConstructionIndicator));

	memset(m_szAvailableRBDsString, '\0', sizeof(m_szAvailableRBDsString));	
}



/////////////////////////////////////////////////////////////////////
// Input string may be Flight Segment data line if meets criteria: //
// 1st char MUST be either 'A', 'L', 'O','U', or 'S'.              //
// 2nd char MUST be either '*' or ' '                              //
// AND iff 3rd & 4th chars are alphanumeric.                       //
/////////////////////////////////////////////////////////////////////
bool SegmentDataClass::IsProbablySegmentData(const char *p_szInputString)
{
	bool bReturnValue = true;

	if (p_szInputString[0] == 'A' || p_szInputString[0] == 'L' ||
		p_szInputString[0] == 'O' || p_szInputString[0] == 'U' ||
		p_szInputString[0] == 'S')
	{
		if (p_szInputString[1] == '*' || p_szInputString[1] == ' ')
			bReturnValue = true;

		bReturnValue = bReturnValue && isalnum(p_szInputString[2]);
		bReturnValue = bReturnValue && isalnum(p_szInputString[3]);
	}
	else
		bReturnValue = false;

	return bReturnValue;
}

/////////////////////////////////////////////////////////////////////////
// Parses the input string to fill the member variables of this class. //
// Returns true if successful; false if not.                           //
/////////////////////////////////////////////////////////////////////////
bool SegmentDataClass::ParseInputString(const char *p_szInputString)
{
	if (strlen(p_szInputString) > LINESIZE)	// String is too big.
		return false;

	InitMemberVariables();	// In case memory is being re-used.

	char szBuffer[100];		// General purpose scratch buffer.

	strcpy(szBuffer, p_szInputString);

	if (szBuffer[5] == '.') // Kludge: Convert dots to spaces at pos 5.
		szBuffer[5] =  ' '; 

	m_szSegmentStatus[0] = szBuffer[0];
	strncpy(m_szCarrierCode, &szBuffer[2], 3);
	strncpy(m_szFlightNumber,&szBuffer[5], 5); // Length may be changed to 6 in the future (Sept.17, 2001).

	strncpy(m_szRBD, &szBuffer[10], 1); // May be 2 chars in the future; only 1 for now (Sept.17, 2001).
	strncpy(m_szDepartureDate, &szBuffer[11], 5); // May be 6 or 9 chars in the future.

	if ((strcmp(&m_szDepartureDate[2], "JAN") == 0) ||
		(strcmp(&m_szDepartureDate[2], "FEB") == 0) ||
		(strcmp(&m_szDepartureDate[2], "MAR") == 0) ||
		(strcmp(&m_szDepartureDate[2], "APR") == 0)) 
	{
		strcat(m_szDepartureDate, "02");
	}
	else
	{	
		strcat(m_szDepartureDate, "01");
	}

	strncpy(m_szDeparturePoint, &szBuffer[17], 3);
	strncpy(m_szDepartureTime, &szBuffer[20], 4);

	strncpy(m_szDateChangeIndicator, &szBuffer[24], 1);

	// strncpy(m_szArrivalDate, &szBuffer[25], 9);
	strcpy(m_szArrivalDate, m_szDepartureDate);

	strncpy(m_szArrivalTime, &szBuffer[25], 4);
	strncpy(m_szArrivalPoint, &szBuffer[29], 3);

	strncpy(m_szNumberOfStopovers, &szBuffer[32], 1);
	strncpy(m_szStopoverIndicator, &szBuffer[33], 1);

/****
	strncpy(m_szGlobalIndicator, &szBuffer[], 2);
	strncpy(m_szEquipmentType, &szBuffer[], 2);
	strncpy(m_szFareConstructionIndicator, &szBuffer[], 2);

	strncpy(m_szAvailableRBDsString, &szBuffer[], 52);
***/
	return true;
}

void SegmentDataClass::PrintMemberVariables(void)
{
	cout << "SegmentStatus            : [" << m_szSegmentStatus << "]" << endl;
	cout << "CarrierCode              : [" << m_szCarrierCode << "]" << endl;
	cout << "FlightNumber             : [" << m_szFlightNumber << "]" << endl;
	cout << "RBD                      : [" << m_szRBD << "]" << endl;
	
	cout << "DepartureDate            : [" << m_szDepartureDate << "]" << endl;
	cout << "DeparturePoint           : [" << m_szDeparturePoint << "]" << endl;
	cout << "DepartureTime            : [" << m_szDepartureTime << "]" << endl;
	cout << "DateChangeIndicator      : [" << m_szDateChangeIndicator << "]" << endl;

	cout << "ArrivalDate              : [" << m_szArrivalDate << "]" << endl;
	cout << "ArrivalPoint             : [" << m_szArrivalPoint << "]" << endl;
	cout << "ArrivalTime              : [" << m_szArrivalTime << "]" << endl;

	cout << "NumberOfStopovers        : [" << m_szNumberOfStopovers << "]" << endl;
	cout << "StopoverIndicator        : [" << m_szStopoverIndicator << "]" << endl;

	cout << "GlobalIndicator          : [" << m_szGlobalIndicator << "]" << endl;
	cout << "EquipmentType            : [" << m_szEquipmentType << "]" << endl;
	cout << "FareConstructionIndicator: [" << m_szFareConstructionIndicator << "]" << endl;

	cout << "AvailableRBDsString      : [" << m_szAvailableRBDsString << "]" << endl;
}