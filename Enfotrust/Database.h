// Copyright © 2001 - 2005 enfoTrust networks, inc.

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "shared.h"
#include "dblib.h"

//----------------------------------------------------------------------------

class CDBTile : public CDBDatabase
{
	public:

		CDBTile() {}
		virtual ~CDBTile() {}

		virtual HRESULT Initialize();

		virtual CDBCommand * GetCommandByName(const wstring & sCommandName);

	private:

		CDBCommand m_CallInsertCmd;
		CDBCommand m_QuestionnaireInsertCmd;
		CDBCommand m_AnswerInsertCmd;
		CDBCommand m_SignatureInsertOrUpdateCmd;
};

//----------------------------------------------------------------------------

#endif
