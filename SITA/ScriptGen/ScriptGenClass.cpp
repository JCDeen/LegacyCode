/////////////////////////////////////////////////////////////////////////
// ScriptGenClass.cpp: implementation of the ScriptGenClass class.	   //	
//																	   //	
// Documentation source :											   //
// Author: James Christian Deen										   //
/////////////////////////////////////////////////////////////////////////

#include "ScriptGenClass.h"
//#define SHOW_DEBUG_INFO_IN_WEBPAGE_COMMENTS
//////////////////////////////////////////////////////////////////////
// Construction/Destruction 
//////////////////////////////////////////////////////////////////////

ScriptGenClass::ScriptGenClass()
{
	m_nCommandLineDebugMode = 0;

	m_pCityCodeAlpha = ReturnSafeArray();
	m_pCMX = m_pCityName = m_pCityType = m_pCurrencyCode = m_pCountryCode = m_pStateCode = ReturnSafeArray();
	m_pDaylightSavingsTime = m_pMainframeStatusText = ReturnSafeArray();

	memset(m_szGMTDiffPMString, '\0', 10);
	memset(m_szZoneString, '\0', 10);
	memset(m_szRegionString, '\0', 10);

	strcpy(m_szGMTDiffPMString, "?");
	strcpy(m_szZoneString, "?");
	strcpy(m_szRegionString, "?");

	m_pLatitude = m_pLongitude = m_pLonEastWest = m_pAreaCode = m_pArrivalTax = m_pDepartureTax = m_pCityIsRural = ReturnSafeArray();

	for (int i = 0; i < 5; i++)
		m_pCityAirportCodeA[i] = m_pCityAirportNameA[i] = m_pCityAirportCodeB[i] = m_pCityAirportNameB[i] = ReturnSafeArray();

	SetErrorStatus(EVERYTHING_IS_FINE);
	m_bError = false;
	m_nDebug = 0;
}


// .................................................
// Function name	: ScriptGenClass::~ScriptGenClass
// Description	    : 
// Return type		: 
// .................................................
ScriptGenClass::~ScriptGenClass()
{
}

/////////////////////////////////////
// Gets data into m_szTransBuffer. //
/////////////////////////////////////

// .................................................
// Function name	: ScriptGenClass::Simulated_GetDataFromMainframe
// Description	    : 
// Return type		: void 
// .................................................
void ScriptGenClass::Simulated_GetDataFromFile()
{
	memset(m_szTransBuffer, '\0', USABLE_BUFFER_SIZE);

	strcat(m_szTransBuffer, "DBFATL            \r\n");
	strcat(m_szTransBuffer, "CTY/iata5/THIS IS SIMULATED/ TYP /Q/CLA/\r\n\r\n"); // Double CR/LF on purpose.
	strcat(m_szTransBuffer, "CUR/000/COU/885/12345/ABCDEFG/ST/GA/MPM/ /\r\n");
	strcat(m_szTransBuffer, "GMT/M0500/Y/GEO/    1/111   1/************\r\n");
	strcat(m_szTransBuffer, "LAT/21581/LON/+*14736/ZO/11/TA/0/TD/0/ /  \r\n");
	strcat(m_szTransBuffer, "COD/AIRPORT         /COD/AIRPORT         /\r\n"); // Header line only.
	strcat(m_szTransBuffer, "111/Dallas,Ft.Worth /112/NewJersey,      /\r\n");
	strcat(m_szTransBuffer, "200/Tanzania,Africa /211/NewGuinea,Paupau/\r\n");
	strcat(m_szTransBuffer, "300/Barbados,Nothere/311/St.GeorgesIsland/\r\n");
	strcat(m_szTransBuffer, "400/Navarre,Florida /411/St.Johns,USVI   /\r\n");
	strcat(m_szTransBuffer, "500/EglinAFB,Florida/521/St.Croix,USVI   /\r\n");

	char szETX[2];
	sprintf(szETX, "%c", ETX);
	strcat(m_szTransBuffer, szETX);

	m_bError = false;
}

void ScriptGenClass::Pre_ParseDataFromInputFile()
{
	int i;
	m_bError = true;
	char *pStartLine, *pEndLine;

	/////////////////////////////////////////////////
	// Get Page Count & Ticket/Display if present. //
	// Expected to be on last line returned from   //
	// transaction, but not required.		       //	
	/////////////////////////////////////////////////
	//m_pPageCurrent = PointToNextField(m_szTransBuffer,"/PAGE");
	//m_pPageTotal = PointToNextField(m_pPageCurrent,"/");
	//m_pDisplayOrTicket = PointToNextField(m_szTransBuffer,"/FCC=");

	///////////////////////////
	// Get data from line 1. //
	///////////////////////////
	pStartLine = PointToNextLine(m_szTransBuffer); // "CTY/"
	m_pCityCodeAlpha = PointToNextField(m_szTransBuffer,"DBF", false);
	if (*(m_pCityCodeAlpha + 3) == '/')
		m_pCMX = m_pCityCodeAlpha + 4;	// Example: "DBFATL/M"
	else
		m_pCMX = strdup("G");			// No '/' found after "DBFxyz", so get city file by default.
	*(m_pCityCodeAlpha + 3) = EOT;		// Flag for conversion to '\0'.

	///////////////////////////
	// Get data from line 2. //
	///////////////////////////
	m_pCityName	= PointToNextField(pStartLine,"CTY/     /", false); // Skip to next '/'
	m_pCityType	= PointToNextField(m_pCityName,"/ TYP /");
	pEndLine = PointToNextField(m_pCityName,"/CLA");

	///////////////////////////
	// Get data from line 3. //
	///////////////////////////
	pStartLine = PointToNextLine(pEndLine);
	m_pCurrencyCode	= PointToNextField(pStartLine, "CUR/");
	m_pCountryCode	= PointToNextField(m_pCityName,"/COU/");
	m_pStateCode	= PointToNextField(m_pCountryCode,"/12345/ABCDEFG/ST/");
	pEndLine = PointToNextField(m_pStateCode, "/MPM/ /"); 

	///////////////////////////
	// Get data from line 4. //
	///////////////////////////
	pStartLine = PointToNextLine(pEndLine);
	char *pGMTDiffPM = PointToNextField(pStartLine ,"GMT/"); 
	for (i = 0; i< 5; i++)
		m_szGMTDiffPMString[i] = *(pGMTDiffPM + i); 

	m_pDaylightSavingsTime = PointToNextField(pGMTDiffPM,"/");

	char *pZone = PointToNextField(m_pDaylightSavingsTime,"/GEO/");
	for (i = 0; i< 5; i++)
		m_szZoneString[i] = *(pZone + i); 

	char *pRegion = PointToNextField(pZone,"/");
	for (i = 0; i< 7; i++)
		m_szRegionString[i] = *(pRegion + i); 
	m_szRegionString[7] = '\0';
	pEndLine = PointToNextField(m_pStateCode, "/***"); 

	///////////////////////////
	// Get data from line 5. //
	///////////////////////////
	pStartLine = PointToNextLine(pEndLine);
	m_pLatitude	= PointToNextField(pStartLine,"LAT/", false);
	m_pLonEastWest = PointToNextField(m_pLatitude, "/LON/");
	m_pLongitude = PointToNextField(m_pLonEastWest,"*");

	m_pAreaCode	= PointToNextField(m_pLongitude,"/ZO/");

	m_pArrivalTax = PointToNextField(m_pLongitude,"/TA/");
	m_pDepartureTax = PointToNextField(m_pLongitude,"/TD/");

	m_pCityIsRural = PointToNextField(m_pDepartureTax,"/");
	pEndLine = PointToNextField(m_pCityIsRural,"/");

	////////////////////////////////////////
	// Codes for multiple airport cities. //
	// Rows:6-10 Cols: a=left; b = right  //
	////////////////////////////////////////

	pEndLine = PointToNextLine(pEndLine);		// Skip over header line: COD/AIRPORT        /COD/AIRPORT

	m_pCityAirportCodeA[0]	= PointToNextLine(pEndLine);
	m_pCityAirportNameA[0]	= PointToNextField(m_pCityAirportCodeA[0], 4);
	m_pCityAirportCodeB[0]	= PointToNextField(m_pCityAirportNameA[0],17);
	m_pCityAirportNameB[0]	= PointToNextField(m_pCityAirportCodeB[0], 4);
	PointToNextField(m_pCityAirportNameB[0], 17);	// Replaces '/' at end of field with EOT.

	m_pCityAirportCodeA[1]	= PointToNextLine(m_pCityAirportNameB[0]);
	m_pCityAirportNameA[1]	= PointToNextField(m_pCityAirportCodeA[1], 4);
	m_pCityAirportCodeB[1]	= PointToNextField(m_pCityAirportNameA[1],17);
	m_pCityAirportNameB[1]	= PointToNextField(m_pCityAirportCodeB[1], 4);
	PointToNextField(m_pCityAirportNameB[1], 17);	// Replaces '/' at end of field with EOT.

	m_pCityAirportCodeA[2]	= PointToNextLine(m_pCityAirportNameB[1]);
	m_pCityAirportNameA[2]	= PointToNextField(m_pCityAirportCodeA[2], 4);
	m_pCityAirportCodeB[2]	= PointToNextField(m_pCityAirportNameA[2],17);
	m_pCityAirportNameB[2]	= PointToNextField(m_pCityAirportCodeB[2], 4);
	PointToNextField(m_pCityAirportNameB[2], 17);	// Replaces '/' at end of field with EOT.
	
	m_pCityAirportCodeA[3]	= PointToNextLine(m_pCityAirportNameB[2]);
	m_pCityAirportNameA[3]	= PointToNextField(m_pCityAirportCodeA[3], 4);
	m_pCityAirportCodeB[3]	= PointToNextField(m_pCityAirportNameA[3],17);
	m_pCityAirportNameB[3]	= PointToNextField(m_pCityAirportCodeB[3], 4);
	PointToNextField(m_pCityAirportNameB[3], 17);	// Replaces '/' at end of field with EOT.

	m_pCityAirportCodeA[4]	= PointToNextLine(m_pCityAirportNameB[3]);
	m_pCityAirportNameA[4]	= PointToNextField(m_pCityAirportCodeA[4], 4);
	m_pCityAirportCodeB[4]	= PointToNextField(m_pCityAirportNameA[4],17);
	m_pCityAirportNameB[4]	= PointToNextField(m_pCityAirportCodeB[4], 4);
	pEndLine = PointToNextField(m_pCityAirportNameB[4], 17);	// Replaces '/' at end of field with EOT.

	pStartLine = PointToNextLine(pEndLine);
	m_pMainframeStatusText = strdup(pStartLine);

	//////////////////////////////////////////////////////////
	// Replacing '\n', '\r', '*' and '/' with '\0' lets the //
	// data members above point to zero terminated strings. //
	//////////////////////////////////////////////////////////

	for (int nPos = 0; nPos < USABLE_BUFFER_SIZE; nPos++)
	{	char c = m_szTransBuffer[nPos];
		if (c == '\n' || c == '\r' || c == '*' || c == EOT || c == ETX)
			m_szTransBuffer[nPos] = '\0';
		else if (c == ' ' && m_szTransBuffer[nPos+1] == ' ')	/// KEEP AN EYE ON THIS! JCD
			m_szTransBuffer[nPos] = '\0';
	}

	m_bError = false;
}

/////////////////////////////////////////////////////////
// Assumes member variables populated after a call to  //
// ParseDataFromWebpage() or ParseDataFromMainframe(). //
/////////////////////////////////////////////////////////
void ScriptGenClass::BuildOutputToFile()
{
	char szBuf[100];	// Scratch buffer.
	char szMF[15][50];	// 15 rows x 50 chars.
	m_bError = false;

	//////////////////
	// Build line 1 //
	//////////////////
	if (*m_pCMX == 'G')
		sprintf(szMF[0], "DBF%3s\r\n", m_pCityCodeAlpha);
	else
		sprintf(szMF[0], "DBF%3s/%c\r\n", m_pCityCodeAlpha, *m_pCMX);

	//////////////////
	// Build line 2 //
	//////////////////
	strcpy(szBuf, m_pCityName);
	sprintf(szMF[1], "CTY/     /%-17.17s/ TYP /%c/CLA/\r\n", szBuf, *m_pCityType);

	//////////////////
	// Build line 3 //
	//////////////////
	sprintf(szMF[2], "CUR/%3s/COU/%3s/12345/ABCDEFG/ST/%2s/MPM/ /\r\n", 
		m_pCurrencyCode, m_pCountryCode, m_pStateCode);

	//////////////////
	// Build line 4 //
	//////////////////
	sprintf(szMF[3], "GMT/%5s/%c/GEO/%5s/%7s/************\r\n",
		m_szGMTDiffPMString, *m_pDaylightSavingsTime, m_szZoneString, m_szRegionString);

	//////////////////
	// Build line 5 //
	//////////////////
	if (*m_pCityIsRural != 'R')
	   *m_pCityIsRural = ' ';	// Make sure space is there

	if (*m_pLonEastWest == 'W')
		*m_pLonEastWest = '+';
	else
		*m_pLonEastWest = '&';

	sprintf(szMF[4], "LAT/%5s/LON/%c*%5s/ZO/%2s/TA/%c/TD/%c/%c/\r\n",
		m_pLatitude, *m_pLonEastWest, m_pLongitude, m_pAreaCode, *m_pArrivalTax, *m_pDepartureTax,
		*m_pCityIsRural); // szRural);

	////////////////////////////////
	// Build line 6 - Header only //
	////////////////////////////////
	// ruler comment ///////////123456789/          /123456789/
	strcpy(szMF[5], "COD/AIRPORT         /COD/AIRPORT         /\r\n");

	////////////////////////
	// Build lines 7 - 11 //
	////////////////////////
	int nLine;
	for (nLine = 0; nLine < 5; nLine++)
		sprintf(szMF[nLine + 6], "%3s/%-16.16s/%3s/%-16.16s/\r\n", 
			m_pCityAirportCodeA[nLine], m_pCityAirportNameA[nLine],
			m_pCityAirportCodeB[nLine], m_pCityAirportNameB[nLine]);

	////////////////////////////////////////////////////
	// Copy 11 line input data to transaction buffer. //
	////////////////////////////////////////////////////
	memset(m_szTransBuffer, '\0', ACTUAL_BUFFER_SIZE);
	for (nLine = 0; nLine < 11; nLine++)
		strcat(m_szTransBuffer, szMF[nLine]);
	sprintf(szBuf, "%c", ETX);
	strcat(m_szTransBuffer, szBuf);			// Append ETX.

	m_bError = false;
}

// .................................................
// Function name	: ScriptGenClass::ParseDataFromWebPage
// Description	    : 
// Return type		: void 
// .................................................
void ScriptGenClass::ParseDataFromInputFile()
{
//	cgiHexToAscii(m_szTransBuffer);	// Convert hex chars back to Ascii.
	
	strcat(m_szTransBuffer, "&"); // Appending '&' eliminates parsing dependence on order of arguments.

	if (strstr(m_szTransBuffer,"ENABLE_TRACE=YES") != NULL)
		m_bEnableTrace = true;
	else
		m_bEnableTrace = false;

	if (strstr(m_szTransBuffer,"USE_FARES_BANNER=YES") != NULL)
		m_bUseFaresBanner = true;
	else
		m_bUseFaresBanner = false;

	////////////////// CMX values and their meaning /////////////////
	// 'C'=Create; 'M'=Modify; 'X' = Delete; 'G'=Get new city file //
	/////////////////////////////////////////////////////////////////
	m_pCityCodeAlpha = PointToNextField(m_szTransBuffer,"CTY=", false);
	m_pCMX = PointToNextField(m_szTransBuffer,"CMX=", false);		// C,M,X or G.

	/////////////////////////////////////////
	// If no city code entered then error. //
	/////////////////////////////////////////
	if (*m_pCityCodeAlpha == THE_SAFE_CHAR)
	{	SetErrorStatus(DERIVED_CLASS_ERROR1); // No city code
		m_bError = false;
		return;
	}

	/////////////////////////////////////////////////
	// If CMX == 'G' then get new city file data.  //
	// No need to parse data from web page further.//
	/////////////////////////////////////////////////
	if (*m_pCMX == 'G')
		return;

	m_pCityName	= PointToNextField(m_szTransBuffer,"CNAME=", false);
	m_pCityType	= PointToNextField(m_szTransBuffer,"CTYPE=", false);

	m_pCurrencyCode	= PointToNextField(m_szTransBuffer,"CURRENCY=", false);
	m_pCountryCode	= PointToNextField(m_szTransBuffer,"COUNTRY=", false);
	m_pStateCode	= PointToNextField(m_szTransBuffer,"STATE=", false);

	//char *pGMTDiff, *pGMTPM;

	char *pGMTDiff = PointToNextField(m_szTransBuffer,"GMTDIFF=", false);
	char *pGMTPM   = PointToNextField(m_szTransBuffer,"GMTPM=", false);
	m_szGMTDiffPMString[0] = *pGMTPM;
	m_szGMTDiffPMString[1] = *pGMTDiff;
	m_szGMTDiffPMString[2] = *(pGMTDiff+1);
	m_szGMTDiffPMString[3] = *(pGMTDiff+2);
	m_szGMTDiffPMString[4] = *(pGMTDiff+3);
	m_szGMTDiffPMString[5] = '\0';

	m_pDaylightSavingsTime = PointToNextField(m_szTransBuffer,"DST=", false);

	m_szZoneString[0]	= *PointToNextField(m_szTransBuffer,"Z1=", false);
	m_szZoneString[1]	= *PointToNextField(m_szTransBuffer,"Z2=", false);
	m_szZoneString[2]	= *PointToNextField(m_szTransBuffer,"Z3=", false);
	m_szZoneString[3]	= *PointToNextField(m_szTransBuffer,"Z4=", false);
	m_szZoneString[4]	= *PointToNextField(m_szTransBuffer,"Z5=", false);
	m_szZoneString[5]	= '\0';

	for (int nZ=0; nZ<5; nZ++)
		if (m_szZoneString[nZ] == '?')
			m_szZoneString[nZ] = ' ';

	m_szRegionString[0] = *PointToNextField(m_szTransBuffer,"RA=", false);
	m_szRegionString[1] = *PointToNextField(m_szTransBuffer,"RB=", false);
	m_szRegionString[2] = *PointToNextField(m_szTransBuffer,"RC=", false);
	m_szRegionString[3] = *PointToNextField(m_szTransBuffer,"RD=", false);
	m_szRegionString[4] = *PointToNextField(m_szTransBuffer,"RE=", false);
	m_szRegionString[5] = *PointToNextField(m_szTransBuffer,"RF=", false);
	m_szRegionString[6] = *PointToNextField(m_szTransBuffer,"RG=", false);
	m_szRegionString[7] = '\0';

	for (int nR=0; nR<7; nR++)
		if (m_szRegionString[nR] == '?')
			m_szRegionString[nR] = ' ';

	m_pLatitude	= PointToNextField(m_szTransBuffer,"LAT=", false);
	m_pLongitude = PointToNextField(m_szTransBuffer,"LONG=", false);

	m_pLonEastWest = PointToNextField(m_szTransBuffer, "EASTWEST=", false); // 'E' or 'W'.

	m_pAreaCode	= PointToNextField(m_szTransBuffer,"AREACODE=", false);

	m_pArrivalTax	= PointToNextField(m_szTransBuffer,"ARRIVALTAX=", false);
	m_pDepartureTax = PointToNextField(m_szTransBuffer,"DEPARTURETAX=", false);

	m_pCityIsRural = PointToNextField(m_szTransBuffer,"RURALCITY=", false);

	////////////////////////////////////////
	// Codes for multiple airport cities. //
	// 6-10 = Row; Col: A=left; B = right //
	////////////////////////////////////////

	m_pCityAirportCodeA[0]	= PointToNextField(m_szTransBuffer,"C6A=", false);
	m_pCityAirportNameA[0]	= PointToNextField(m_szTransBuffer,"N6A=", false);
	m_pCityAirportCodeB[0]	= PointToNextField(m_szTransBuffer,"C6B=", false);
	m_pCityAirportNameB[0]	= PointToNextField(m_szTransBuffer,"N6B=", false);

	m_pCityAirportCodeA[1]	= PointToNextField(m_szTransBuffer,"C7A=", false);
	m_pCityAirportNameA[1]	= PointToNextField(m_szTransBuffer,"N7A=", false);
	m_pCityAirportCodeB[1]	= PointToNextField(m_szTransBuffer,"C7B=", false);
	m_pCityAirportNameB[1]	= PointToNextField(m_szTransBuffer,"N7B=", false);

	m_pCityAirportCodeA[2]	= PointToNextField(m_szTransBuffer,"C8A=", false);
	m_pCityAirportNameA[2]	= PointToNextField(m_szTransBuffer,"N8A=", false);
	m_pCityAirportCodeB[2]	= PointToNextField(m_szTransBuffer,"C8B=", false);
	m_pCityAirportNameB[2]	= PointToNextField(m_szTransBuffer,"N8B=", false);

	m_pCityAirportCodeA[3]	= PointToNextField(m_szTransBuffer,"C9A=", false);
	m_pCityAirportNameA[3]	= PointToNextField(m_szTransBuffer,"N9A=", false);
	m_pCityAirportCodeB[3]	= PointToNextField(m_szTransBuffer,"C9B=", false);
	m_pCityAirportNameB[3]	= PointToNextField(m_szTransBuffer,"N9B=", false);

	m_pCityAirportCodeA[4]	= PointToNextField(m_szTransBuffer,"C10A=", false);
	m_pCityAirportNameA[4]	= PointToNextField(m_szTransBuffer,"N10A=", false);
	m_pCityAirportCodeB[4]	= PointToNextField(m_szTransBuffer,"C10B=", false);
	m_pCityAirportNameB[4]	= PointToNextField(m_szTransBuffer,"N10B=", false);

	//// Ampersand Replacement Loop" /////
	// Replacing '&' with '\0' lets the //
	// data members above point to zero //
	// terminated strings.              //
	//////////////////////////////////////

	for (int nPos = 0; nPos < USABLE_BUFFER_SIZE; nPos++)
	{	char c = m_szTransBuffer[nPos];
		if (c == '&')
			m_szTransBuffer[nPos] = '\0'; // Zero terminator
		else if(c == '+')
			m_szTransBuffer[nPos] = ' '; // Space
	}

	m_bError = false;
}

