	LoginRequestData stLoginRequest;

	strcpy(stLoginRequest.szUserId, pstrUserID);
	strcpy(stLoginRequest.szPassword, pstrPassword);
	strcpy(stLoginRequest.szSystem, pstrSystem);

	char szClientID[MAX_CLIENTID_LEN+1];
	strcpy(szClientID, "");

	if (ProcessRequest(LOGIN_REQUEST, szClientID, &stLoginRequest) == NO_WAFER_ERROR)
	{

		////////////////////////////////////////////////////////////////////////////////
		// JCD - Should TAKE THIS OUT LATER. Should be handled sooner than here ! JCD //
		// JCD - Should TAKE THIS OUT LATER. Should be handled sooner than here ! JCD //
		////////////////////////////////////////////////////////////////////////////////
		if(m_pClientManager->VerifyClient(szClientID) == NULL)
		{
			StartContent(pCtxt);
			WriteTitle(pCtxt);

			*pCtxt << "<H2>VerifyClient(" << szClientID << ")== NULL)<BR>\r\n";

			CString cs;
			cs.Format("Has %d chars",strlen(szClientID));
			*pCtxt << cs;

			EndContent(pCtxt);
		}
		else
		{

			*pCtxt << "<!DOCTYPE HTML PUBLIC \"-//W3O//DTD W3 HTML 3.0//EN\">";

			*pCtxt << "<HTML>\r\n";
			*pCtxt << "<HEAD>\r\n";
			*pCtxt << "<TITLE>WAFER</TITLE>\r\n";
			*pCtxt << "</HEAD>\r\n";

			*pCtxt << "<frameset rows=\"40%, *\" border=0 frameborder=0>\r\n";
			*pCtxt << "<frame src=\"/cgi-bin/IsapiServer.dll?GetPage&ClientID=" << szClientID; //csClientID;
			*pCtxt << "\" name=\"Header\" scrolling=\"AUTO\" border=0 frameborder=0></frame> \r\n";

			*pCtxt <<  "<frame src=\"welcome.htm\" name=\"Main\" scrolling=\"AUTO\" border=0 frameborder=0</frame>\r\n";
			*pCtxt <<  "<noframes>\r\n";
			*pCtxt << "<BODY>\r\n";
			*pCtxt << "    <p>This web page uses frames, but your browser doesn't support them.</p>\r\n";
			*pCtxt << "</BODY>\r\n";
			*pCtxt << "    </noframes>\r\n";
			*pCtxt << "</frameset>\r\n";
			*pCtxt << "</HTML>";
		}
	}
	else
	{
		StartContent(pCtxt);
		WriteTitle(pCtxt);

		*pCtxt << "<H1>Unable to Login for :</H1>\n";
		*pCtxt << "pstrSystem  : " << stLoginRequest.szSystem   <<"<BR>\n";
		*pCtxt << "pstrUserID  : " << stLoginRequest.szUserId   <<"<BR>\n";
		*pCtxt << "pstrPassword: " << stLoginRequest.szPassword <<"<BR>";

		EndContent(pCtxt);
	}
	
