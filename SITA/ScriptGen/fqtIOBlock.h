// fqtIOBlock.h: interface for the fqtIOBlock class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FQTIOBLOCK_H__2CFB5699_B087_4A8F_9773_2B1B28E0F706__INCLUDED_)
#define AFX_FQTIOBLOCK_H__2CFB5699_B087_4A8F_9773_2B1B28E0F706__INCLUDED_

#include <stdio.h>		// Move to an include file
#include <stdlib.h>		// For exit();
#include <string.h>

#include "ScriptGen.h"

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

class fqtIOBlock  
{
	char m_szTransactionCmdLine[LINESIZE];
	//char m_sz

public:
	fqtIOBlock();
	virtual ~fqtIOBlock();

};

#endif // !defined(AFX_FQTIOBLOCK_H__2CFB5699_B087_4A8F_9773_2B1B28E0F706__INCLUDED_)
