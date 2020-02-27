// SegmentDataClass.h: interface for the SegmentDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENTDATACLASS_H__4E27A6F1_2337_40D4_B41C_830E1BE0243B__INCLUDED_)
#define AFX_SEGMENTDATACLASS_H__4E27A6F1_2337_40D4_B41C_830E1BE0243B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptGen.h"

/////////////////////////////////////////////////////////////////
// DEVELOPMENT NOTES:										   //
//	All member variables below are zero terminated null        //
//	strings for simplicity's sake, even if storing only 1 char.//
/////////////////////////////////////////////////////////////////

class SegmentDataClass  
{
public:
	char m_szSegmentStatus[2];
	char m_szCarrierCode[4];
	char m_szFlightNumber[7];
	char m_szRBD[3];				// Size is 1 char for now. 18Sept2001
	char m_szDepartureDate[10];
	char m_szDeparturePoint[4];
	char m_szDepartureTime[5];
	char m_szDateChangeIndicator[2]; // '<' means -1; '+' means +1; '>' means +2.

	char m_szArrivalDate[10];
	char m_szArrivalPoint[4];
	char m_szArrivalTime[5];

	char m_szNumberOfStopovers[2];
	char m_szStopoverIndicator[2];

	char m_szGlobalIndicator[3];
	char m_szEquipmentType[3];
	char m_szFareConstructionIndicator[3];

	char m_szAvailableRBDsString[53];		// Treat this as an array of 52 chars.

	SegmentDataClass();
	virtual ~SegmentDataClass();

	bool ParseInputString(const char *p_szInputString);
	void PrintMemberVariables(void);						//Mostly for debugging/development.
	bool IsProbablySegmentData(const char *p_szInputString);

private:
	void InitMemberVariables(void);
};
#endif // !defined(AFX_SEGMENTDATACLASS_H__4E27A6F1_2337_40D4_B41C_830E1BE0243B__INCLUDED_)
