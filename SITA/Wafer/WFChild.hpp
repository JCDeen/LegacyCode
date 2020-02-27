#include "Portable.h"
#include "Wip.h"
#include "WFClient.hpp"

// ----------------------------------------------------------------------------
// class WaferChild: This class is responsible for communicating to the
// Transaction server / mainframe and passing on the results to the "client"
// connected to the wafer server.
// ----------------------------------------------------------------------------
class WaferChild {
	WaferSocket*		m_pSocket; // The socket associated with a client
	MFSockInterface*	m_pInterface; // The interface to the mainframe.
	WIP_RequestType		m_eRequestType;
	WIP_ClientType		m_eClientType;
	Int_t				m_ExitStatus;
	string				m_szClientID; // The client's ID in the collection that
									  // the Wafer Server maintains.
public:

	WaferChild(WaferClient* pWaferClient);
	~WaferChild() {}

	void Begin(void);

	void Process();
	void ProcessLogin(void);
	void ProcessExpert(void);
	void ProcessLogout(void);

	void ProcessFares(void);
	void ProcessRules(void)
	{
		cout << "Process Rules not implemented yet..\n";
	}
	
	void ProcessParagraphs(void)
	{
		cout << "Process Paragraphs not implemented yet..\n";
	}
	
	void ProcessFootNotes(void)
	{
		cout << "Process Foot Notes not implemented yet..\n";
	}
	
	// ----------------------------------------------------------------------------
	// Function name	: SetError
	// Description	    : This function "OR"s the existing error code with the
	//                    exit status. The error codes are typically like 1, 2, 4
	//                    8, 32, 64 & so on. This will help to track each error that
	//                    the WaferChild encountered. Could be useful for making
	//                    more detailed analysis later...
	// Return type		: void 
	// Argument         : int nErr - The error code that the wafer child encountered.
	// ----------------------------------------------------------------------------
	void SetError(int nErr)
	{
		m_ExitStatus |= nErr;
	}
	
	void Exit(void);
};
