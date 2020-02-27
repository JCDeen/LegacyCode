// MonthTableClass.h: interface for the MonthTableClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONTHTABLECLASS_H__2D2FD4C7_BC3C_4771_B53E_F4B6FEEE149A__INCLUDED_)
  #define AFX_MONTHTABLECLASS_H__2D2FD4C7_BC3C_4771_B53E_F4B6FEEE149A__INCLUDED_

  #if _MSC_VER >= 1000
 	#pragma once
  #endif // _MSC_VER >= 1000

#include "ScriptGen.h"

struct MonthTableData
{
	char m_szMonth[4];
	int	 m_nDaysInMonth;
};

class MonthTableClass  
{
private:
	 MonthTableData MonthTable[12];

public:
	MonthTableClass();
	virtual ~MonthTableClass();

	char *getMonthNameByIndex(int nIndex);
	int getDaysInMonthByIndex(int nIndex);

	bool b_IndexIsValid;

};

#endif // !defined(AFX_MONTHTABLECLASS_H__2D2FD4C7_BC3C_4771_B53E_F4B6FEEE149A__INCLUDED_)
