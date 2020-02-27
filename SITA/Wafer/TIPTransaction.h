#ifndef _TIPTRANSACTION_DEFINED
#define _TIPTRANSACTION_DEFINED

#include "Transaction.h"
#include "WFSocket.hpp"
#include "Tip.h"
#include "Wip.h"

// ----------------------------------------------------------------------------
// class TIPTransaction: This is used when a transaction server is available
//                       for communicating to tha mainframe. Packets
//                       between the Wafer server and the Transaction server
//                       would then be in TIP format.
// ----------------------------------------------------------------------------
class TIPTransaction : public Transaction {
	WaferClientSocket*	m_pTransSocket;
	BOOL				m_bLoggedIn;
	BOOL				m_bActive;
public:
	TIPTransaction();
	~TIPTransaction();

	virtual BOOL Initialize(void);
	virtual BOOL Login(LoginRequestData* pstLoginRequest);
	virtual char* Fares(FaresRequestData* pstFaresRequest);
	virtual char* Expert(ExpertRequest* pstExpertRequest);
	virtual BOOL Logout();

	BOOL IsLoggedIn()
	{
		return m_bLoggedIn;
	}
	
	BOOL IsActive()
	{
		return m_bActive;
	}
};

#endif