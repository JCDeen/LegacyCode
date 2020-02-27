#ifndef TRANSACTION
#define TRANSACTION

#include "Wip.h"
#include "Portable.h"

// ----------------------------------------------------------------------------
// class Transaction: Generic transaction class.
// ----------------------------------------------------------------------------
class Transaction {
public:
	Transaction() {}
	virtual ~Transaction() {}

	virtual BOOL Login(LoginRequestData* pLoginRequest) = 0;
	virtual char* Fares(FaresRequestData* pFaresRequest) = 0;
	virtual char* Expert(ExpertRequest* pExpertRequest) = 0;
	virtual BOOL Logout(void) = 0;

	virtual BOOL ChangeSystem(char* szSystem) = 0;
};
#endif
