// Copyright © 2001 - 2005 enfoTrust networks, inc.

#include "stdafx.h"

#include "shared.h"
#include "service.h"
#include "dblib.h"
#include "database.h"
#include "snoopy.h"

#include "error.h"

static char fileversion[] = "$Header: /Source/Snoopy/SnoopyMain.cpp 8     3/15/04 2:43p Cdeen $";

//----------------------------------------------------------------------------
//////////////////////////////////////////////////
// JCD                                          //
//////////////////////////////////////////////////

int _tmain(int argc, LPCTSTR argv[])
{
	HRESULT hr;

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 2921;

	CSnoopy snoopy;
	hr = snoopy.Main(argc, argv);	// Inherited from CService --- JCD

	_CrtDumpMemoryLeaks();

	return hr;
}

//----------------------------------------------------------------------------
