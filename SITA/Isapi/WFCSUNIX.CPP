#include "WFCSUnix.h"


// ----------------------------------------------------------------------------
// Function name	: WFCS_Unix::WFCS_Unix
// Description	    : Constructor.To add new signals to be handled, add the
//                    lines:
//                    sigaddset(&m_tSigSetMask, SIGNAL_TO_BE_HANDLED);
// Return type		: None
// ----------------------------------------------------------------------------
WFCS_Unix::WFCS_Unix()
{
	sigaddset(&m_tSigSetMask, SIGINT);
	sigaddset(&m_tSigSetMask, SIGCHLD);

	m_bLocked = 0;
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Unix::~WFCS_Unix
// Description	    : Destructor. Deleting the WaferCritical section only stops
//                    the capability of the server to block and unblock the
//                    signals.
// Return type		: None.
// ----------------------------------------------------------------------------
WFCS_Unix::~WFCS_Unix()
{
	if (m_bLocked)
		UnLock();
	m_bLocked = 0;
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Unix::Lock
// Description	    : This method blocks the signals to be raised. To add new
//                    signals to be blocked, add the lines:
//                    sigaction(SIGNAL_TO_BE_HANDLED, NULL, &m_stSigMyHandle);
// Return type		: void 
// ----------------------------------------------------------------------------
void WFCS_Unix::Lock()
{
	if (!m_bLocked) {
		//
		// Calling sigaction with the second parameter equal to NULL will
		// help in 'reading' the signal action..
		//
		sigaction(SIGINT, NULL, &m_stSigIntAction);
		sigaction(SIGCHLD, NULL, &m_stSigChildAction);

		sigprocmask(SIG_BLOCK, &m_tSigSetMask, NULL);

		m_bLocked = 1;
	}
}


// ----------------------------------------------------------------------------
// Function name	: WFCS_Unix::UnLock
// Description	    : Un block the signals.
// Return type		: void 
// ----------------------------------------------------------------------------
void WFCS_Unix::UnLock()
{
	if (m_bLocked) {
		sigaction(SIGINT, &m_stSigIntAction, NULL);
		sigaction(SIGCHLD, &m_stSigChildAction, NULL);

		sigprocmask(SIG_UNBLOCK, &m_tSigSetMask, NULL);

		m_bLocked = 0;
	}
}

