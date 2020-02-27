#ifndef _UNIX_CRITICALSECTION_DEFINED
#define _UNIX_CRITICALSECTION_DEFINED

#include "Critical.h"

#include <signal.h>
#include <unistd.h>

//
// class WFCS_Unix: This class implements the critical section for Wafer
// in a Unix environment.
//
// Signals are used for tracking events in the Unix version of Wafer
// server. During some critical processing it is required that these signals
// be blocked. This class maintains a "sigaction" structure for each signal
// that can be raised and a sigset_t which describes the mask. The idea is
// to "mask" the signals when the method Lock is called and unmask the signals
// when UnLock is called.
//
class WFCS_Unix : public WFCriticalSection {
private:
	struct sigaction	m_stSigIntAction;
	struct sigaction	m_stSigChildAction;
	sigset_t			m_tSigSetMask;

	int					m_bLocked;
public:
	WFCS_Unix();
	~WFCS_Unix();

	virtual void Lock();
	virtual void UnLock();
};

#endif
