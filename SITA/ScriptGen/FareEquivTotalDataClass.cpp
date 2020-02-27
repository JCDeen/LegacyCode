// FareEquivTotalDataClass.cpp: implementation of the FareEquivTotalDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "FareEquivTotalDataClass.h"

#define AMOUNT_STRING_LEN 8

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FareEquivTotalDataClass::FareEquivTotalDataClass()
{
	InitMemberVariables();
}

FareEquivTotalDataClass::~FareEquivTotalDataClass()
{
}

void FareEquivTotalDataClass::InitMemberVariables(void)
{
	memset(m_szFareCurrencyCode, '\0', sizeof(m_szFareCurrencyCode));
	memset(m_szEquivCurrencyCode, '\0', sizeof(m_szEquivCurrencyCode));
	memset(m_szTotalCurrencyCode, '\0', sizeof(m_szTotalCurrencyCode));
	memset(m_szTaxCurrencyCode, '\0', sizeof(m_szTaxCurrencyCode));

	memset(m_szFareAmountAsString, '\0', sizeof(m_szFareAmountAsString));
	memset(m_szEquivAmountAsString, '\0', sizeof(m_szEquivAmountAsString));
	memset(m_szTotalAmountAsString, '\0', sizeof(m_szTotalAmountAsString));
	memset(m_szTaxAmountAsString, '\0', sizeof(m_szTaxAmountAsString));

	m_bEquivTextIsPresent = false;
	m_bTaxLineIsPresent = false;
}

/////////////////////////////////////////////
// Load up the FARE line member variables. //
/////////////////////////////////////////////
bool FareEquivTotalDataClass::ParseFareInputString(const char *p_szInputString)
{
	if (strlen(p_szInputString) > LINESIZE)	// String is too big.
		return false;

	if (strncmp(p_szInputString, "FARE ", 5) !=0)
		return false;

	//////////////////////////////////////////////////////////////////////////////
	// Currency Code & Amount can be determined by position.                    //
	// If Currency Code is "TR" + numeric then shift everything left by 1 char. //
	//////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	// NOTE: Need to make this a function if ANY more logic is added! //
	////////////////////////////////////////////////////////////////////
	strncpy(m_szFareCurrencyCode, &p_szInputString[6], 3);
	if (strncmp(m_szFareCurrencyCode, "TR", 2) ==0)
	{	strcpy(m_szFareCurrencyCode, "TRL");
		strncpy(m_szFareAmountAsString, &p_szInputString[8], AMOUNT_STRING_LEN +1);
	}
	else
		strncpy(m_szFareAmountAsString, &p_szInputString[9], AMOUNT_STRING_LEN);

	char *pEquivText = strstr(p_szInputString, "EQUIV");
	if (pEquivText != NULL)
	{	m_bEquivTextIsPresent = true;
		strncpy(m_szEquivCurrencyCode, pEquivText + 7, 3);
		strncpy(m_szEquivAmountAsString, pEquivText + 10, 8);
	}

	return true;
}

////////////////////////////////////////////
// Load up the TAX line member variables. //
////////////////////////////////////////////
bool FareEquivTotalDataClass::ParseTaxInputString(const char *p_szInputString)
{
	if (strlen(p_szInputString) > LINESIZE)	// String is too big.
		return false;

	if (strncmp(p_szInputString, "TAX ", 4) !=0)
		return false;

	m_bTaxLineIsPresent = true;

	///////////////////////////////////////////////////////////
	// Currency Code & Amount can be determined by position. //
	///////////////////////////////////////////////////////////
	//strncpy(m_szTaxCurrencyCode, &p_szInputString[6], 3);
	//strncpy(m_szTaxAmountAsString, &p_szInputString[9], 8);

	////////////////////////////////////////////////////////////////////
	// NOTE: Need to make this a function if ANY more logic is added! //
	////////////////////////////////////////////////////////////////////
	strncpy(m_szTaxCurrencyCode, &p_szInputString[6], 3);
	if (strncmp(m_szTaxCurrencyCode, "TR", 2) ==0)
	{	strcpy(m_szTaxCurrencyCode, "TRL");
		strncpy(m_szTaxAmountAsString, &p_szInputString[8], AMOUNT_STRING_LEN + 1);
	}
	else
		strncpy(m_szTaxAmountAsString, &p_szInputString[9], AMOUNT_STRING_LEN);

	return true;
}

/////////////////////////////////////////////
// Load up the TOTAL line member variables.//
/////////////////////////////////////////////
bool FareEquivTotalDataClass::ParseTotalInputString(const char *p_szInputString)
{
	if (strlen(p_szInputString) > LINESIZE)	// String is too big.
		return false;

	if (strncmp(p_szInputString, "TOTAL ", 6) !=0)
		return false;

	///////////////////////////////////////////////////////////
	// Currency Code & Amount can be determined by position. //
	///////////////////////////////////////////////////////////
	//strncpy(m_szTotalCurrencyCode, &p_szInputString[6], 3);
	//strncpy(m_szTotalAmountAsString, &p_szInputString[9], 8);

	////////////////////////////////////////////////////////////////////
	// NOTE: Need to make this a function if ANY more logic is added! //
	////////////////////////////////////////////////////////////////////
	strncpy(m_szTotalCurrencyCode, &p_szInputString[6], 3);
	if (strncmp(m_szTotalCurrencyCode, "TR", 2) ==0)
	{	strcpy(m_szTotalCurrencyCode, "TRL");
		strncpy(m_szTotalAmountAsString, &p_szInputString[8], AMOUNT_STRING_LEN + 1);
	}
	else
		strncpy(m_szTotalAmountAsString, &p_szInputString[9], AMOUNT_STRING_LEN);

	return true;
}

bool FareEquivTotalDataClass::bIsFareLine (const char *p_szInputString)
{
	if (strncmp(p_szInputString, "FARE ", 5) ==0)
		return true;
	else
		return false;
}

bool FareEquivTotalDataClass::bIsTaxLine (const char *p_szInputString)
{
	if (strncmp(p_szInputString, "TAX ", 4) ==0)
	{	m_bTaxLineIsPresent = true;
		return true;
	}
	else
		return false;
}

bool FareEquivTotalDataClass::bIsTotalLine (const char *p_szInputString)
{
	if (strncmp(p_szInputString, "TOTAL ", 6) ==0)
		return true;
	else
		return false;
}

void FareEquivTotalDataClass::PrintMemberVariables(void)
{
	cout << "m_szFareCurrencyCode   [" <<  m_szFareCurrencyCode << "]" << endl;
	cout << "m_szEquivCurrencyCode  [" <<  m_szEquivCurrencyCode << "]" << endl;
	cout << "m_szTotalCurrencyCode  [" <<  m_szTotalCurrencyCode << "]" << endl;
	cout << "m_szTaxCurrencyCode    [" <<  m_szTaxCurrencyCode << "]" << endl;

	cout << "m_szFareAmountAsString [" <<  m_szFareAmountAsString << "]" << endl;
	cout << "m_szEquivAmountAsString[" <<  m_szEquivAmountAsString << "]" << endl;
	cout << "m_szTotalAmountAsString[" <<  m_szTotalAmountAsString << "]" << endl;
	cout << "m_szTaxAmountAsString  [" <<  m_szTaxAmountAsString << "]" << endl;

	cout << "m_bEquivTextIsPresent  [" << m_bEquivTextIsPresent << "]" << endl;
	cout << "m_bTaxLineIsPresent    [" << m_bTaxLineIsPresent << "]" << endl;
}

