#include "WFCSWin.h"

// ----------------------------------------------------------------------------
// Function name	: WFCS_Win32::WFCS_Win32
// Description	    : Constructor for a critical section.
// Return type		: None
// ----------------------------------------------------------------------------
WFCS_Win32::WFCS_Win32()
{
	InitializeCriticalSection(&m_CriticalSection);
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Win32::~WFCS_Win32
// Description	    : Destructor for the critical section.
// Return type		: None.
// ----------------------------------------------------------------------------
WFCS_Win32::~WFCS_Win32()
{
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Win32::Lock
// Description	    : Locks the critical section
// Return type		: void 
// ----------------------------------------------------------------------------
void WFCS_Win32::Lock()
{
	EnterCriticalSection(&m_CriticalSection);
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Win32::UnLock
// Description	    : Releases the Critical section.
// Return type		: void 
// ----------------------------------------------------------------------------
void WFCS_Win32::UnLock()
{
	LeaveCriticalSection(&m_CriticalSection);
}

