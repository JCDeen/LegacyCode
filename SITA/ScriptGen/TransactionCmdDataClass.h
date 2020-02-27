// TransactionCmdDataClass.h: interface for the TransactionCmdDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSACTIONCMDDATACLASS_H__F7C0C4F0_1391_41E8_90FC_7E8EAAF929DA__INCLUDED_)
#define AFX_TRANSACTIONCMDDATACLASS_H__F7C0C4F0_1391_41E8_90FC_7E8EAAF929DA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptGen.h"

/////////////////////////////////////////////////////////////////
// DEVELOPMENT NOTES:										   //
//	All member variables below are zero terminated null        //
//	strings for simplicity's sake, even if storing only 1 char.//
/////////////////////////////////////////////////////////////////

class TransactionCmdDataClass  
{
public:
	char m_szTransactionCmd[4];
	char m_szSalesCity[4];
	char m_szTicketingCity[4];

	bool m_bTicketingCityIsPresent;

	TransactionCmdDataClass();
	virtual ~TransactionCmdDataClass();

	bool ParseInputString(const char *p_szInputString);
	void PrintMemberVariables(void);					//Mostly for debugging/development.
};

#endif // !defined(AFX_TRANSACTIONCMDDATACLASS_H__F7C0C4F0_1391_41E8_90FC_7E8EAAF929DA__INCLUDED_)
