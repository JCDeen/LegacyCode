///////////////////////////////////////////////////////////////////
// Program:	ScriptGen                                            //
// Author:	Chris Deen                                           //
// Purpose:                                                      //
///////////////////////////////////////////////////////////////////

#include "ScriptGen.h"
#include <MonthTableClass.h>
#include <SegmentDataClass.h>
#include <TransactionCmdDataClass.h>
#include <FareEquivTotalDataClass.h>

///////////////////////////////////////////////////////////////////
// DEVELOPMENT NOTES (From Dorothy Aenlle)                       //
//		NO MORE THAN 28 FLIGHT SEGMENTS PLUS 3 SURFACE SEGMENTS. //
//		BASE FOR AIRFARE-FQT AND ITA-QPX IS SET AT 01MAY01 12:00 //
//		SITI="SOLD INSIDE, TICKETED INSIDE" THE COUNTY OF ORIGIN.// 
//		SOTI="SOLD OUTSIDE, TICKETED INSIDE"                     // 
///////////////////////////////////////////////////////////////////

#define FARE_LINE_READING_LIMIT 50
#define COMMENTS_SKIP_LIMIT		10

void main (int argc, char *argv[]);
bool bIsNewDataBlock(const char *p_szInputString); // Defined at bottom.

void main (int argc, char *argv[])
{
	char szInputFilename[50], szOutputFilename[50];
	char szMode[20];			// should be "-leg" or "-journey"
	char szClientString[50];
	char szUserString[50];
	char szCurrentTimeString[20];
	char szOutputFormatString[50];
	char szAircraftType[4];

	char  *szCurrencyDecimalTable [][2]=
	{
		{"BHD","3"},
		{"KWD","3"},
		{"OMR","3"},
		{"YER","3"},
		{"JOD","3"},
		{"LYD","3"},
		{"TND","3"},
		{"ITL","0"},
		{"JPY","0"},
		{"TRL","0"},
		{"MZM","0"}
	};

	/////////////////////////////////////////////////////////////////////
	// Get the input/output & config file names from the command line. //
	// The config filename is optional.                                //
	// A default config file will be used if not specified.            //
	/////////////////////////////////////////////////////////////////////
	if (argc >= 2)
	{
		strcpy(szInputFilename, argv[1]);

		if (argc >=4 )
			strcpy(szOutputFilename, argv[3]);

		if (argc >=3 )
			strcpy(szMode, argv[2]);

		/*** for debugging *** JCD
			for (int ij=0; ij < argc; ij++)
				cout << "argv[" << ij << "] = <" << argv[ij] << ">" << endl;
		***/
	}
	else
	{
		cout << "======= Scriptgen Information ==================== " << endl;
		cout << "Executable Name               : " << argv[0] << endl;
		cout << "ScriptGen.cpp Last Compiled On: " << __TIMESTAMP__ << endl;
		cout << endl;
		cout << "======= ScriptGen Usage Notes ==================== " << endl;
		cout << "In a Dos box on Windows, type ScriptGen followed by the input file name:" << endl;
		cout << "      scriptgen inputfile.txt" << endl << endl;
		cout << "   The QPX output will then be printed on your screen (in the Dos box)." << endl << endl;
		cout << "   To route the output to a filename, do this:" << endl;
		cout << "      scriptgen inputfile.txt > outputfile.txt" << endl << endl;
		cout << "   Notes:" << endl;
		cout << "      (1) Be sure to include a space before and after the \">\" symbol." << endl;
		cout << "      (2) Add \"-journey\" to use ScriptGen in \"SPECIFIED-JOURNEY\" mode" << endl;
		cout << "          (Default mode is \"SPECIFIED-LEG\" mode. )" << endl;
		cout << "          Example: scriptgen inputfile.txt -journey >> outputfile.txt" << endl;

		exit (0);
	}

/**
	ofstream ofFile("bogusfile", ios::out);
	ofFile.close();
**/

	///////////////////////////////////////////
	// Display the command line arguments to //
	// the user iff DEBUG_OUTPUT is defined. //
	///////////////////////////////////////////
	#ifdef DEBUG_OUTPUT
		// cout << "Input  File: " << szInputFilename << endl;
		// cout << "Output File: " << szOutputFilename << endl;
	#endif

	ifstream InputFile(szInputFilename, ios::in | ios::nocreate);
	InputFile.setmode(filebuf::text);	// Set file input mode to text.

	if (!InputFile)
	{
		InputFile.close();
		cerr << "OPENING THE INPUT FILE [" << szInputFilename << "] FAILED\n";
		cerr << "Please notify your favorite programmer of the problem.\n";
		exit(-1);
	}

	//////////////////////////////////////////////////////////////////////
	// Init "constant" strings. (Get these from a config file later...) //
	//////////////////////////////////////////////////////////////////////
	strcpy(szClientString, "TEXT");
	strcpy(szUserString, "fares.helpdesk@sita.int");
	strcpy(szOutputFormatString, "TOP-N 1 CHEAPEST PRETTY PRINT");
	strcpy(szCurrentTimeString, "01MAY01 12:00");
	strcpy(szAircraftType, "JET");

	char szBuffer[100]; // General purpose input buffer.
	memset(szBuffer, '\0', sizeof(szBuffer));

	///////////////////////////////////////////////////////////
	// Had to put these inits here to satisfy compiler since //
	// I used a GOTO to exit for certain error conditions.   //
	///////////////////////////////////////////////////////////
	int nInputFileLineNumber = 0;
	int nFQT_BlockNumber	 = 0;
	int nLineCount = 0;
	int nCounter = 0;
	int nLineNum = 0;
	int nDecimalCount = 2;
	int nArrayIndex = 0;

	char *pDecimalPt;

	int nNumZeros = 0;

	MonthTableClass monthTable;
	SegmentDataClass flightSeg[28];
	FareEquivTotalDataClass fareLineData;	// Init here because GOTO may skip it.

	/////////////////////// Sample QPX Output for one "code block" ////////////////////
	// Sample Output: 
	// QUERY                        PRICE-FLIGHT-SEGMENTS
	// QUERY-ID                     1-0010 CURR
	// CLIENT                       TEXT
	// USER                         fares.helpdesk@sita.int
	// CURRENT-TIME                 01MAY01 12:00
	// NUMBER-OF-FLIGHT-SEGMENTS    2
	// SALES-CITY                   AMM
	// SPECIFIED-LEG                0 RJ   100 AMM AQJ 12JUL01  08:00 12JUL01 09:00 JET
	// SPECIFIED-SEGMENT            0 0
	// FLIGHT-SEGMENT-BOOKING-CODES 0 Y
	// SPECIFIED-LEG                1 RJ   200 AQJ AMM 19JUL01 10:00 19JUL01 11:00 JET
	// SPECIFIED-SEGMENT            1 1
	// FLIGHT-SEGMENT-BOOKING-CODES 1 Y
	// ASSERT-PRICE   1000          "LOC Base Fare JOD 60.000" JOD 3 60000
	// OUTPUT-FORMAT                TOP-N 1 CHEAPEST PRETTY-PRINT


	///////////////////////////////////////////
	// Get the first line of input from file.//
	///////////////////////////////////////////

	while (!InputFile.eof()) 
	{	InputFile.getline(szBuffer, 90);
		if (bIsNewDataBlock(szBuffer))
		{	nFQT_BlockNumber++;
			break;
		}
	}

	while (!InputFile.eof()) 
	{
		cout << "%%" << &szBuffer[2] << endl;	// Output the first line as a QPX comment.
		
		//////////////////////////////////////////
		// Output the next 2 lines of QPX code..//
		////////////////////////////////////////////////////////
		// QUERY                        PRICE-FLIGHT-SEGMENTS //
		// QUERY-ID                     1-0010 CURR           //
		////////////////////////////////////////////////////////

		cout << "QUERY                        PRICE-FLIGHT-SEGMENTS" << endl;
	
		//////////////////////////////////////////////////////////
		// Examples as guide to design logic for QUERY-ID line. //
		//                                                      //
		//12345678901234567890123456789012345678901234567890    //
		//00000000011111111112222222222333333333344444444445    //
		//                                                      //
		//**12-0010 CURR SITO TRANSACTION                       //
		//**1-0020 CURR-SITI TRANSACTION WHOLLY DOMESTIC** ?    // 
		//**12-0030 CURR-SOTO                                   //
		//**12-0030 CAT109-SOTO                                 //
		//**2-0030 CAT109-SITI                                  //
		//**1-0040 CURR                                         // 
		//////////////////////////////////////////////////////////

		// Replace any 2nd occurence of '-' with null zero.
		char *p2ndHyphen = strchr(&szBuffer[5], '-');
		if (p2ndHyphen != NULL)
			*p2ndHyphen = '\0';

		szBuffer[16] = '\0';	// No need to print anything past element 16.

		// Replace any 2nd occurence of ' ' with null zero.
		char *p2ndSpace = strchr(&szBuffer[12], ' ');
		if (p2ndSpace != NULL)
			*p2ndSpace = '\0';

		// szBuffer[] is now ready to print, starting with char # 2.

		cout << "QUERY-ID                     " << &szBuffer[2] << endl;

		cout << "CLIENT                       TEXT" << endl;
		cout << "USER                         fares.helpdesk@sita.int" << endl;
		cout << "CURRENT-TIME                 01MAY01 12:00" << endl;

		////////////////////////////////////////////
		// Get the Transaction Cmd line of input. //
		////////////////////////////////////////////
		InputFile.getline(szBuffer, 90);
		TransactionCmdDataClass xactionCmd;
		if (!xactionCmd.ParseInputString(szBuffer))
		{
			cout << "%%WARNING: ===============================================" << endl;
			cout << "%%         Trouble Parsing For Transaction Cmd Line Input." << endl;
			cout << "%%         SKIPPING TO NEXT INPUT DATA BLOCK" << endl;
			cout << "%%         ===============================================" << endl;
			goto SkipToNextDataBlock;
		}


		////////////////////////////////////////////
		// Get the Flight Segment lines of input. //
		////////////////////////////////////////////
		for (nLineNum = 0; nLineNum < 28; nLineNum++)
		{	
			memset(szBuffer, '\0', sizeof(szBuffer)); // Perhaps delete this line later? JCD
		
			InputFile.getline(szBuffer, 90);

			if(!flightSeg[nLineNum].IsProbablySegmentData(szBuffer))
			{	
				if (nLineNum == 0)
				{	cout << "%%WARNING: =========================" << endl;
					cout << "%%         NO SEGMENT DATA FOUND ..." << endl;
					cout << "%%         =========================" << endl;
				}
				break;
			}

			flightSeg[nLineNum].ParseInputString(szBuffer);
		}

		cout << "NUMBER-OF-FLIGHT-SEGMENTS    " << nLineNum << endl;
		cout << "SALES-CITY                   " << xactionCmd.m_szSalesCity << endl;

		if (xactionCmd.m_bTicketingCityIsPresent)
			cout << "TICKETING-CITY               " << xactionCmd.m_szTicketingCity << endl;

		///////////////////////////////////////////////////
		// Build the Flight Segment lines of QPX output. //
		////////////////// example: ///////////////////////
        // SPECIFIED-LEG                0 RJ   100 AMM AQJ 12JUL01  08:00 12JUL01 09:00 JET
        // SPECIFIED-SEGMENT            0 0
	    // FLIGHT-SEGMENT-BOOKING-CODES 0 Y

		for (nCounter = 0; nCounter < nLineNum; nCounter++)
		{
			///////////////////////
			// Date Change Logic //
			///////////////////////
			if (flightSeg[nCounter].m_szDateChangeIndicator[0] != ' ')
			{
				int n_DateChangeOffset = 0;
				switch (flightSeg[nCounter].m_szDateChangeIndicator[0])
				{	case '>':
						n_DateChangeOffset = 1;
						break;
					case '+':
						n_DateChangeOffset = 2;
						break;
					case '<':
						n_DateChangeOffset = -1;
						break;
				}

				char szDate[3], szMonth[4], szYear[3];
				strncpy(szDate, flightSeg[nCounter].m_szArrivalDate, 2);
				szDate[2] = '\0';

				strncpy(szMonth, &flightSeg[nCounter].m_szArrivalDate[2], 3);
				szMonth[3] = '\0';

				strncpy(szYear, &flightSeg[nCounter].m_szArrivalDate[5], 2);
				szYear[2] = '\0';

				int changedDate = atoi(szDate) + n_DateChangeOffset;

				int nMonthIndex = 0;
				for (nMonthIndex = 0; nMonthIndex <= 11; nMonthIndex++)
				{	
					if (strnicmp(monthTable.getMonthNameByIndex(nMonthIndex), &flightSeg[nCounter].m_szArrivalDate[2], 3) == 0)
					{	
						if (changedDate > monthTable.getDaysInMonthByIndex(nMonthIndex))
						{	
							strcpy(szDate, "01");	// New date will be 1st of month.

							if (nMonthIndex == 11)	// 31DEC01 --> 01JAN02
							{
								strcpy(szMonth, "JAN");

								if (strcmp(szYear, "01") == 0)
									strcpy(szYear, "02");
								else if (strcmp(szYear, "02") == 0)
									strcpy(szYear, "03");
								else
									cout << "%% WARNING: NOT programmed for Year > 2003 !!!" << endl;
							}
							else // JAN..NOV --> FEB..DEC
							{
								strcpy(szMonth, monthTable.getMonthNameByIndex(nMonthIndex + 1));
							}
						}
						else if (changedDate < 1) // Backup one Month
						{
							if (nMonthIndex == 0)	// 31JAN02 --> 01DEC01
							{
								strcpy(szMonth, "DEC");
								strcpy(szDate, "31");

								if (strcmp(szYear, "02") == 0)
									strcpy(szYear, "01");
								else if (strcmp(szYear, "03") == 0)
									strcpy(szYear, "02");
								else
									cout << "%% WARNING: NOT programmed for Year < 2002 !!!" << endl;
							}
							else // JAN..NOV --> 
							{
								strcpy(szMonth, monthTable.getMonthNameByIndex(nMonthIndex - 1));
								sprintf(szDate, "%02d", monthTable.getDaysInMonthByIndex(nMonthIndex - 1));
							}
						}
						else // Handle as normal ...
						{
							sprintf(szDate, "%02d", changedDate);
						}
						break;
					}
				}
				strcpy(flightSeg[nCounter].m_szArrivalDate, szDate);
				strcat(flightSeg[nCounter].m_szArrivalDate, szMonth);
				strcat(flightSeg[nCounter].m_szArrivalDate, szYear);
			}
			///////////////////////////////
			// End of Date Change Logic. //
			///////////////////////////////

			cout << "SPECIFIED-LEG                " << nCounter;
			cout << " " << flightSeg[nCounter].m_szCarrierCode << flightSeg[nCounter].m_szFlightNumber;
			cout << " " << flightSeg[nCounter].m_szDeparturePoint;
			cout << " " << flightSeg[nCounter].m_szArrivalPoint;
			cout << " " << flightSeg[nCounter].m_szDepartureDate;
			cout << " " << flightSeg[nCounter].m_szDepartureTime;
			cout << " " << flightSeg[nCounter].m_szArrivalDate;
			cout << " " << flightSeg[nCounter].m_szArrivalTime << " JET" << endl;

			cout << "SPECIFIED-SEGMENT            " << nCounter << " " << nCounter << endl;
			cout << "FLIGHT-SEGMENT-BOOKING-CODES " << nCounter << " " << flightSeg[nCounter].m_szRBD << endl;

			if (strlen(flightSeg[nCounter].m_szGlobalIndicator) > 0)
				cout << "%% WARNING: Date Change Indicator is not used yet." << endl;
		}

		///////////////////////////////////////////////
		// Get the FARE line of input.               //
		// Loop until find line starting with "FARE" //
		///////////////////////////////////////////////

		nLineCount = 0;
		while (!fareLineData.bIsFareLine(szBuffer))
		{	if (nLineCount++ > FARE_LINE_READING_LIMIT)
			{
  				cout << "%%WARNING: ===============================================================" << endl;
				cout << "%%         LINE STARTING WITH \"FARE\" NOT FOUND WITHIN PROGRAMMED LIMITS." << endl;
				cout << "%%         SKIPPING TO NEXT INPUT DATA BLOCK" << endl;
  				cout << "%%         ===============================================================" << endl;
				goto SkipToNextDataBlock;
			}
			InputFile.getline(szBuffer, 90);
		}

		fareLineData.ParseFareInputString(szBuffer);

		////////////////////////////////////////////////////////////////
		// If next input line is TAX line, then parse for TOTAL line. //
		// Else discard it and look for the next block of data.       //
		////////////////////////////////////////////////////////////////
		InputFile.getline(szBuffer, 90);
		if (fareLineData.bIsTaxLine(szBuffer))
		{
			fareLineData.ParseTaxInputString(szBuffer);
			InputFile.getline(szBuffer, 90);
			fareLineData.ParseTotalInputString(szBuffer);
		}
		///////JCD FareInput

		////////////////////////////////////////////////
		// Build the ASSERT-PRICE line of QPX output. //
		////////////////////////////////////////////////
		cout << "ASSERT-PRICE   1000          \"Loc Base Fare ";
		cout << fareLineData.m_szFareCurrencyCode << " ";
		cout << fareLineData.m_szFareAmountAsString << "\" ";


		char szCurrencyAmountOutput[CURRENCY_AMOUNT_STRING_SIZE];
		char szCurrencyCodeOutput[CURRENCY_CODE_STRING_SIZE];

		if (fareLineData.m_bTaxLineIsPresent)
		{
			strcpy(szCurrencyCodeOutput, fareLineData.m_szTotalCurrencyCode);
			strcpy(szCurrencyAmountOutput, fareLineData.m_szTotalAmountAsString);
		}
		else // No Tax Line
		{	if (fareLineData.m_bEquivTextIsPresent)
			{
				strcpy(szCurrencyCodeOutput, fareLineData.m_szEquivCurrencyCode);
				strcpy(szCurrencyAmountOutput, fareLineData.m_szEquivAmountAsString);
			}
			else
			{
				strcpy(szCurrencyCodeOutput, fareLineData.m_szFareCurrencyCode);
				strcpy(szCurrencyAmountOutput, fareLineData.m_szFareAmountAsString);
			}
		}

		////////////////////////////////////////////////////////////
		// Look for the CurrencyCode in this table. If not found, //
		// then use "2" as the default for DecimalCount.          //
		////////////////////////////////////////////////////////////
		cout << szCurrencyCodeOutput;
		nDecimalCount = 2;
		for (nArrayIndex = 0; nArrayIndex < 10; nArrayIndex++)
		{	if (strcmp(szCurrencyCodeOutput, szCurrencyDecimalTable[nArrayIndex][0]) == 0) 
			{	nDecimalCount = atoi(szCurrencyDecimalTable[nArrayIndex][1]);
				break;
			}
		}
			
		cout << " " << nDecimalCount << " ";

		/////////////////////////////////////////////////////////////////////
		// Modify the Currency Amount to show appropriate number of zeros. //
		/////////////////////////////////////////////////////////////////////
		pDecimalPt = strchr(szCurrencyAmountOutput, '.');
		if (pDecimalPt != NULL)
		{	
			*pDecimalPt = '\0';
			cout << szCurrencyAmountOutput;
			cout << pDecimalPt + 1;
		}
		else
		{
			cout << szCurrencyAmountOutput;
			for (int n = 0; n < nDecimalCount; n++)
				cout << "0";
		}
		cout << endl;

		
		//////////////////////////////////////////////////
		// Last line of QPX output for this code block. //
		//////////////////////////////////////////////////
		cout << "OUTPUT-FORMAT                TOP-N 1 CHEAPEST PRETTY-PRINT" << endl;

		///////////////////////////////////////////
		// Get the first line of input.          //
		// Loop until EOF or new block of input. //
		///////////////////////////////////////////
SkipToNextDataBlock:
		fareLineData.InitMemberVariables();

		while (!InputFile.eof()) 
		{	InputFile.getline(szBuffer, 90);
			if (bIsNewDataBlock(szBuffer))
			{	nFQT_BlockNumber++;
				break;
			}
		}
		cout << endl;
	}

	InputFile.close();
}

///////////////////////////////////////////////////////////////////////
// Tests the input string buffer to see if it delimits a new block   //
// of data to parse. Made it a function to reduce bulkiness of code. //
///////////////////////////////////////////////////////////////////////
bool bIsNewDataBlock(const char *p_szInputString)
{	if (p_szInputString[0]=='*' && p_szInputString[1]=='*'&& 
		(p_szInputString[3]=='-' || p_szInputString[4]=='-'))
			return true;
		else
			return false;
}

/********* ISTREAM Public Member Input Functions 
get			Extracts characters from the stream up to, but not including, delimiters.
getline		Extracts characters from the stream (extracts and discards delimiters).
read		Extracts data from the stream.
ignore		Extracts and discards characters.
peek		Returns a character without extracting it from the stream.
gcount		Counts the characters extracted in the last unformatted operation.
eatwhite	Extracts leading white space.
*************************************************/

