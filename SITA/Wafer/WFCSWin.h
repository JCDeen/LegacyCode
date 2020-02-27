#ifndef _WIN32_CRITICAL_SECTION
#define _WIN32_CRITICAL_SECTION

#include <windows.h>
#include <process.h>

#include "Critical.h"

// ----------------------------------------------------------------------------
// class WFCS_Win32: A Wafer Critical section class for Win32 based application
// Right now, the capabilities of this class is very limited. Could be made
// more robust as the project evolves..
// ----------------------------------------------------------------------------

class WFCS_Win32 : public WFCriticalSection {
private:
	CRITICAL_SECTION m_CriticalSection;
public:
	WFCS_Win32();
	~WFCS_Win32();

	virtual void Lock();
	virtual void UnLock();
};

#endif
