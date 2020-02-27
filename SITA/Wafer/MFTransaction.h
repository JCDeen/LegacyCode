#ifndef MFTRANSACTION_DEFINED
#define MFTRANSACTION_DEFINED

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "MFInterface.h"
#include "Transaction.h"

#define HT		9
#define VT		11
#define SO		14
#define DC3		19
#define EM		25
#define ESC		27
#define FS		28
#define SOE		30
#define US		31
#define SPACE	32
#define EP		33
#define DQUOTE	34
#define NUM		35

class MFTransaction : public Transaction {
private:
	MFSockInterface*	m_pMFInterface;
	char*				m_pszMFOut;
public:
	MFTransaction(MFSockInterface* pIfce)
	{
		m_pszMFOut = NULL;
		m_pMFInterface = pIfce;
	}
	virtual ~MFTransaction()
	{
		//
		// Do not delete the socket interface. It has to be saved
		// for future commn. to the mainframe.
		// The MFSocket interface has to be deleted by the class
		// that created it.
		//
		delete m_pszMFOut;
	}

	virtual BOOL Login(LoginRequestData* pLoginRequest);
	virtual char* Fares(FaresRequestData* pFaresRequest) { return NULL;}
	virtual char* Expert(ExpertRequest* pExpertRequest);
	virtual BOOL Logout(void);

	virtual BOOL ChangeSystem(char* szSystem);
public:
	char*	Execute(char *szTransCommand, int nMaxWaitSeconds = 0, int fMessageWait = 0); 
	BOOL	LookForString(char* szOutput, int nOutputLen, char* szFind);
	char*	ParseMFResponse(long lOutBufLen);
	char*	ParseMFResponse2(long lOutBufLen);
};

#endif
