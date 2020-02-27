// FareEquivTotalDataClass.h: interface for the FareEquivTotalDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAREEQUIVTOTALDATACLASS_H__336A31BB_1041_4267_BEE6_F6C5BE3BA8C5__INCLUDED_)
#define AFX_FAREEQUIVTOTALDATACLASS_H__336A31BB_1041_4267_BEE6_F6C5BE3BA8C5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptGen.h"

/////////////////////////////////////////////////////////////////
// DEVELOPMENT NOTES:										   //
//	All member variables below are zero terminated null        //
//	strings for simplicity's sake, even if storing only 1 char.//
/////////////////////////////////////////////////////////////////

#define CURRENCY_AMOUNT_STRING_SIZE 15
#define CURRENCY_CODE_STRING_SIZE 4


class FareEquivTotalDataClass  
{
public:
	char m_szFareCurrencyCode[CURRENCY_CODE_STRING_SIZE];
	char m_szEquivCurrencyCode[CURRENCY_CODE_STRING_SIZE];
	char m_szTotalCurrencyCode[CURRENCY_CODE_STRING_SIZE];
	char m_szTaxCurrencyCode[CURRENCY_CODE_STRING_SIZE];

	char m_szFareAmountAsString[CURRENCY_AMOUNT_STRING_SIZE];
	char m_szEquivAmountAsString[CURRENCY_AMOUNT_STRING_SIZE];
	char m_szTotalAmountAsString[CURRENCY_AMOUNT_STRING_SIZE];
	char m_szTaxAmountAsString[CURRENCY_AMOUNT_STRING_SIZE];

	bool m_bEquivTextIsPresent;
	bool m_bTaxLineIsPresent;

	FareEquivTotalDataClass();
	virtual ~FareEquivTotalDataClass();

	void InitMemberVariables(void);

	bool ParseFareInputString(const char *p_szInputString);
	bool ParseTaxInputString(const char *p_szInputString);
	bool ParseTotalInputString(const char *p_szInputString);

	bool bIsFareLine (const char *p_szInputString);
	bool bIsTaxLine (const char *p_szInputString);
	bool bIsTotalLine (const char *p_szInputString);

	void PrintMemberVariables(void);					//Mostly for debugging/development.

/** May use this later ... JCD

	struct CurrencyDecimalLookup
	{	char szCurrencyCode[4];
		int	 nDecimalPlaces;
	};
**/

};

#endif // !defined(AFX_FAREEQUIVTOTALDATACLASS_H__336A31BB_1041_4267_BEE6_F6C5BE3BA8C5__INCLUDED_)
