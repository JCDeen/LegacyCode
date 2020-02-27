// TransactionCmdDataClass.cpp: implementation of the TransactionCmdDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "TransactionCmdDataClass.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TransactionCmdDataClass::TransactionCmdDataClass()
{
	memset(m_szTransactionCmd, '\0', sizeof(m_szTransactionCmd));
	memset(m_szSalesCity,	   '\0', sizeof(m_szSalesCity));
	memset(m_szTicketingCity,  '\0', sizeof(m_szTicketingCity));

	m_bTicketingCityIsPresent = false;
}

TransactionCmdDataClass::~TransactionCmdDataClass()
{
}

bool TransactionCmdDataClass::ParseInputString(const char *p_szInputString)
{
	if (strlen(p_szInputString) > LINESIZE)	// String is too big.
		return false;

	strncpy(m_szTransactionCmd, &p_szInputString[0], 3);

	//char *p_1stSlash, *p_2ndSlash, *p_3rdSlash;
	char *p_3rdSlash;
	p_3rdSlash = strrchr(&p_szInputString[0], '/');

	if (p_3rdSlash != NULL)
	{	strncpy(m_szSalesCity, p_3rdSlash + 1, 3);

		if (strlen(p_3rdSlash + 4) >= 4)
		{	strncpy(m_szTicketingCity, p_3rdSlash + 5, 3);
			m_bTicketingCityIsPresent = true;
		}
	}
	else
		return false;
   
	return true;
}

void TransactionCmdDataClass::PrintMemberVariables(void)
{
	cout << "m_szTransactionCmd : [" << m_szTransactionCmd << "]" << endl;
	cout << "m_szSalesCity      : [" << m_szSalesCity << "]" << endl;
	cout << "m_szTicketingCity  : [" << m_szTicketingCity << "]" << endl;
}
