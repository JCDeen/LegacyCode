#include <iostream.h>

#include "MFTransaction.h"

BOOL MFTransaction::Login(LoginRequestData* pLoginRequest)
{
	int		nReturnValue;
	char	szMFCmd[80];
	char	szTemp[MAXBUF+1];
	char	szUserIdPassword[50];

	TRACE("MFTransaction::Login\n");

	if (m_pMFInterface->ConnectToServer() == FALSE) {
		return FALSE;
	}
	sprintf(szUserIdPassword, "%s/%s", pLoginRequest->szUserId, pLoginRequest->szPassword);

	// ------------------------------------------------------------------------------
	//
	// The mainframe could send a lot of information before sending 
	// "TELNET: ENTER ATTACH COMMAND". Receive all the buffer sent by the mainframe.
	//
	// -------------------------------------------------------------------------------
	for(;;) {
		memset(szTemp, 0, MAXBUF);
		nReturnValue = m_pMFInterface->Receive(szTemp, MAXBUF);

		if(LookForString(szTemp, nReturnValue, "TELNET: ENTER ATTACH COMMAND") == TRUE) { 
			memset(szMFCmd, 0, 80);
			sprintf(szMFCmd, "attach %s,,P%d\n", MAINFRAME_DCP, m_pMFInterface->GetPID());
			TRACE("MFTransaction::Login - Sending command <attach %s,,P %d>\n", MAINFRAME_DCP, m_pMFInterface->GetPID());

			nReturnValue = m_pMFInterface->Send(szMFCmd, strlen(szMFCmd));

			break;
		}
		else { // Echo back the response received from the mainframe.
			nReturnValue = m_pMFInterface->Send(szTemp, nReturnValue);
		}
	}
	nReturnValue = m_pMFInterface->Receive(szTemp, MAXBUF);

	if (LookForString(szTemp, nReturnValue, "TELNET: ATTACH SUCCESSFUL") == FALSE) {
		TRACE("MFTransaction::Login - Attach failed:Mainframe response: %s\n", szTemp);
		return FALSE;
	}

	if (LookForString(szTemp, nReturnValue, "USERID/PASSWORD") == FALSE) {
		nReturnValue = m_pMFInterface->Receive(szTemp, MAXBUF);
	}

	int pass_prompt = 0;

	while(!pass_prompt) {
		if (strstr(szTemp, "USERID/PASSWORD") != NULL)
			pass_prompt = 1;

		if (pass_prompt == 0) {
			memset(szTemp, 0, MAXBUF);
			nReturnValue = m_pMFInterface->Receive(szTemp, MAXBUF);
			TRACE("MFTransaction::Login - Waiting for Password prompt..\n");
		}
	}
	sprintf(szMFCmd,"%s\n", szUserIdPassword);
	nReturnValue = m_pMFInterface->Send(szMFCmd, strlen(szMFCmd));

	// ---------------------------------------------------------------------------
	// Verify if the user has been properly logged in or not.
	// Wait for MAXWAITSECONDS seconds for a response. If there isn't one, we can
	// safely assume(?) that the Login failed!!
	// ---------------------------------------------------------------------------

	if (m_pMFInterface->Peek(MAXWAITSECONDS) == FALSE)
		return FALSE;

	memset(szTemp, 0, MAXBUF);
	nReturnValue = m_pMFInterface->Receive(szTemp, MAXBUF);

	return LookForString(szTemp, nReturnValue, "Logged in");
}

BOOL MFTransaction::ChangeSystem(char* szSystem)
{
	char	szBuffer[MAXBUF+1];
	int		nReturnValue, nLen;

	TRACE("MFTransaction::ChangeSystem\n");

	sprintf(szBuffer, "//%s\n", szSystem);
	nLen = strlen(szBuffer);
	m_pMFInterface->Send(szBuffer, nLen);

	memset(szBuffer, 0, MAXBUF);

	if (m_pMFInterface->Peek(MAXWAITSECONDS) == FALSE) {
		return FALSE;
	}

	if ( (nReturnValue = m_pMFInterface->Receive(szBuffer, MAXBUF)) == -1 ) {
		// Receive failed to get data
		return FALSE;
	}
	TRACE("MFTransaction::Change System <//%s returned <%s>\n", szSystem, szBuffer);

	return LookForString(szBuffer, nReturnValue, "REQUESTED ACTION COMPLETE");
}

// -----------------------------------------------------------------------
//
// Not to be replaced by string functions such as strstr. The output from
// the mainframe seems to contain NULL characters & the string functions
// may not work properly here.
//
// -----------------------------------------------------------------------
BOOL MFTransaction::LookForString(char* szOutput, int nOutputLen, char* szFind)
{
	Int_t	nLenFind;

	TRACE("MFTransaction::LookForString\n");

	nLenFind = strlen(szFind);

	for(int i = 0; i < nOutputLen-nLenFind; i++) {
		if (strncmp(&szOutput[i], szFind, nLenFind) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFTransaction::Logout()
{
	int		nReturnValue;
	char	*szMFCmd = "logoff\n";
	char	szBuffer[MAXBUF+1];

	TRACE("MFTransaction::Logout\n");

	nReturnValue = m_pMFInterface->Send(szMFCmd, strlen(szMFCmd));

	// If mainframe does not send response in 5 seconds, try again
	if (m_pMFInterface->Peek(5) == FALSE) {
		nReturnValue = m_pMFInterface->Send(szMFCmd, strlen(szMFCmd));
	}
	if ( (nReturnValue = m_pMFInterface->Receive(szBuffer, MAXBUF)) == -1 ) {
		// Receive failed to get data
		return 0;
	}
	TRACE("MFTransaction::Logout response : %s\n", szBuffer);

	if (LookForString(szBuffer, nReturnValue, "PEER CLOSING") == FALSE) {
		return LookForString(szBuffer, nReturnValue, "successful");
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Function name	: MFTransaction::Execute
// Description	    : This function executes a given mainframe command and
//                    returns the result.
// Return type		: char* 
//                    The output from the mainframe as a proper string, stripped
//                    of all special characters.
// Argument         : char *szCommand - The mainframe command
// Argument         : int nMaxWaitTime - Maximum time to wait for a response. This
//                                       parameter would be useful only if the next
//                                       parameter is set to "1".
// Argument         : int fMessageWait - Flag to indicate if we need to wait for
//                                       multiple page responses.
// ----------------------------------------------------------------------------
char* MFTransaction::Execute(char *szCommand, int nMaxWaitTime, int fMessageWait)
{
	int		nReturnValue;
	char	szBuffer[MAXBUF+1];
	long	lOutBufLen = 1;
	BOOL	bCompleted = FALSE;

	TRACE("MFTransaction::Execute\n");

	//
	// Before any transaction check to see if some previous transaction
	// results are pending from the mainframe.
	// Receive the results of previous transaction.
	// Decide what to do with it.
	//

	if (m_pMFInterface->Peek() == TRUE) {
		m_pMFInterface->Receive(szBuffer, MAXBUF);
	}

	m_pMFInterface->Send(szCommand, strlen(szCommand));
	m_pMFInterface->Send("\n", 1);

	if (m_pszMFOut != NULL) {
		free (m_pszMFOut);
	}

	m_pszMFOut = (char *) calloc (1, sizeof(char)); 

	do {
		//
		// The next 8/9 lines of code should be removed
		// and the socket option of MFSockInterface
		// should be changed for a max. wait time.
		//
		if (nMaxWaitTime == 0) {
			if (m_pMFInterface->Peek(MAXWAITSECONDS) == FALSE)
				break;
		}
		else {
			if (m_pMFInterface->Peek(nMaxWaitTime) == FALSE)
				break;
		}
		memset(szBuffer, 0, MAXBUF);

		if ( (nReturnValue = m_pMFInterface->Receive(szBuffer, MAXBUF, 1)) == -1 ) {
			bCompleted = TRUE;
			break;
		}

		//
		// Dynamically increase the size of the results output buffer
		//
		m_pszMFOut = (char *)realloc(m_pszMFOut, lOutBufLen+nReturnValue);
		
		//
		// Need to copy it bit by bit, since the mainframe response
		// may contain some NULL characters.
		//

		for (int i = 0; i < nReturnValue; i++) {
			m_pszMFOut[lOutBufLen-1] = szBuffer[i];
			lOutBufLen++;
		}

		if (fMessageWait) {
			if (m_pMFInterface->Peek(nMaxWaitTime) == FALSE)
				bCompleted = TRUE;
		}
		else
			bCompleted = TRUE;

	} while (bCompleted == FALSE);

	// printf("Output buffer length is %d\n", lOutBufLen);

	//
	// Sometimes the mainframe may not send any response at all..
	//
	if (lOutBufLen == 1)
		return NULL;

	return ParseMFResponse2(lOutBufLen);
}

// ----------------------------------------------------------------------------
// Function name	: MFTransaction::ParseMFResponse
// Description	    : This function strips some of the control characters
//                    returned by mainframe and sends back the "clean" buffer.
// Return type		: char* - The clean output buffer
// Argument         : long lOutBufLen - The length of m_pszMFOut - a private
//                    member in MFTransaction.
// ----------------------------------------------------------------------------
char* MFTransaction::ParseMFResponse(long lOutBufLen)
{
	char* pszTemp = new char [lOutBufLen];
	long lCurPos = 0, lCount = 0;

	TRACE("MFTransaction::ParseMFResponse\n");

	while (lCurPos < lOutBufLen-1) {
		switch(m_pszMFOut[lCurPos]) {
		case 27: // Escape character
			switch(m_pszMFOut[lCurPos+1]) {
			case 11: // Vertical block
				lCurPos += 4;
				break;
			default:
				lCurPos += 2;
				break;
			}
			break;

		case 0:  // Null character.
		case 15: // Don't know what this char is.., but removing it anyway!
		case 30: // Start of Entry char
			lCurPos += 1;
			break;

		default:
			pszTemp[lCount++] = m_pszMFOut[lCurPos++];
			break;
		}
	}
	pszTemp[lCount] = '\0';
	// printf("Mainframe response trimmed to %d chars\n", lCount);
	
	free(m_pszMFOut);
	m_pszMFOut = NULL;

	m_pszMFOut = (char *) calloc (lCount+1, sizeof(char));
	memset (m_pszMFOut, 0, lCount);

	for (lCurPos = 0; lCurPos < lCount; lCurPos++) {
		m_pszMFOut[lCurPos] = pszTemp[lCurPos];
	}
	delete [] pszTemp;

	return m_pszMFOut;
}

char* MFTransaction::Expert(ExpertRequest* pExpertRequest)
{
	TRACE("MFTransaction::Expert\n");

	return Execute(pExpertRequest->szTransaction);
}

char* MFTransaction::ParseMFResponse2(long lOutBufLen)
{
	char* pszTemp = new char [lOutBufLen];
	long lCurPos = 0, lCount = 0;

	TRACE("MFTransaction::ParseMFResponse2\n");
	
	while (lCurPos < lOutBufLen-1) {

		switch(m_pszMFOut[lCurPos]) {

		case 0:  // NULL Character.
		case 15: // Don't know what character this one is...
			lCurPos += 2;
			break;

		case ESC: // Handle an escape sequence
			switch(m_pszMFOut[lCurPos+1]) {

			case VT: // Check for vertical tab cursor position sequence <Esc> <VT> Y X <SI>
				lCurPos += 5;
				break;

			case HT: // Check for horizontal tab cursor position sequence <Esc> <HT>
				lCurPos += 2;
				break;
               
			case FS: // Check for file separator sequence <Esc> <FS> 0 or 1
				lCurPos += 3;
				break;
               
			case 'X': // Check for character placement sequence <Esc> X nn
				lCurPos += 3;
				break;

			case 'Y': // Check for emphasis sequence <Esc> Y x
				lCurPos += 3;
				break;

			case 'Z': // Check for graphics sequence <Esc> Z x
				lCurPos += 3;
				break;
               
			default: // Assume it is a one character sequence
				lCurPos += 2;
				break;
			}
			break; // Break for case ESC

		case EM:
			switch(m_pszMFOut[lCurPos+1]) {

			case SPACE:
			case EP:
			case DQUOTE: // Check for a one colour byte FCC sequence <EM> O M N C1
				lCurPos += 5;
				break;
               
			case NUM: // Check for a two colour byte FCC sequence <EM> O M N C1 C2
				lCurPos += 6;
				break;

			default: // Assume it is a no colour byte FCC sequence <EM> M N
				lCurPos += 3;
				break;
			}
			break; // Break for case EM

		case US: // Handle a Unit Separator sequence
			switch(m_pszMFOut[lCurPos+1]) {

			case SPACE:
			case EP:
			case DQUOTE: // Check for a one colour byte FCC sequence <US> O M N C1
				lCurPos += 5;
				break;

			case NUM: // Check for a two colour byte FCC sequence <US> O M N C1 C2
				lCurPos += 6;
				break;

			default: // Assume it is a no colour byte FCC sequence <US> M N
				lCurPos += 3;
				break;
			}
			break; // Break for case US

		case SO: // Handle a Shift Out sequence
			switch(m_pszMFOut[lCurPos+1]) {
               
			case DC3: // Check for a Device Control sequence <SO> DC3 p or q
				lCurPos += 3;
				break;

			default: // Assume it is the start of a four character sequence (eg. <SO> 0410)
				lCurPos += 5;
				break;
			}
			break; // Break for case SO

		case SOE: // Handle Start of entry
			lCurPos += 2;
			break;

		default:
			pszTemp[lCount++] = m_pszMFOut[lCurPos++];
			break;
		} // End of switch
	} // End of while

	pszTemp[lCount] = '\0';
	// printf("Mainframe response trimmed to %d chars\n", lCount);
	
	free(m_pszMFOut);
	m_pszMFOut = NULL;

	m_pszMFOut = (char *) calloc (lCount+1, sizeof(char));
	memset (m_pszMFOut, 0, lCount);

	for (lCurPos = 0; lCurPos < lCount; lCurPos++) {
		m_pszMFOut[lCurPos] = pszTemp[lCurPos];
	}
	delete [] pszTemp;

	return m_pszMFOut;
}
