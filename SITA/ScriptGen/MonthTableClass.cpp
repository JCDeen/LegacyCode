// MonthTableClass.cpp: implementation of the MonthTableClass class.
//
//////////////////////////////////////////////////////////////////////

#include "MonthTableClass.h"

#define INDEX_IS_BELOW_ZERO		-99
#define INDEX_IS_ABOVE_ELEVEN	-100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MonthTableClass::MonthTableClass()
{
	strcpy(MonthTable[0].m_szMonth, "JAN");	MonthTable[0].m_nDaysInMonth = 31;
	strcpy(MonthTable[1].m_szMonth, "FEB");	MonthTable[1].m_nDaysInMonth = 28;
	strcpy(MonthTable[2].m_szMonth, "MAR");	MonthTable[2].m_nDaysInMonth = 31;
	strcpy(MonthTable[3].m_szMonth, "APR");	MonthTable[3].m_nDaysInMonth = 30;
	strcpy(MonthTable[4].m_szMonth, "MAY");	MonthTable[4].m_nDaysInMonth = 31;
	strcpy(MonthTable[5].m_szMonth, "JUN");	MonthTable[5].m_nDaysInMonth = 30;
	strcpy(MonthTable[6].m_szMonth, "JUL");	MonthTable[6].m_nDaysInMonth = 31;
	strcpy(MonthTable[7].m_szMonth, "AUG");	MonthTable[7].m_nDaysInMonth = 31;
	strcpy(MonthTable[8].m_szMonth, "SEP");	MonthTable[8].m_nDaysInMonth = 30;
	strcpy(MonthTable[9].m_szMonth, "OCT");	MonthTable[9].m_nDaysInMonth = 31;
	strcpy(MonthTable[10].m_szMonth,"NOV");	MonthTable[10].m_nDaysInMonth = 30;
	strcpy(MonthTable[11].m_szMonth,"DEC");	MonthTable[11].m_nDaysInMonth = 31;

	b_IndexIsValid = false;
}

MonthTableClass::~MonthTableClass()
{
}

char * MonthTableClass::getMonthNameByIndex(int nIndex)
{
	b_IndexIsValid = false;

	if (nIndex >=0 && nIndex < 12)
	{
		b_IndexIsValid = true;
		return (MonthTable[nIndex].m_szMonth);
	}
	else
		return "BAD";
}


int MonthTableClass::getDaysInMonthByIndex(int nIndex)
{
	b_IndexIsValid = false;

	if (nIndex < 0)
		return INDEX_IS_BELOW_ZERO;
	else if (nIndex > 11)
		return INDEX_IS_ABOVE_ELEVEN;
	else
	{
		b_IndexIsValid = true;
		return (MonthTable[nIndex].m_nDaysInMonth);
	}
}
