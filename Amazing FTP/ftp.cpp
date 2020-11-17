#include "stdafx.h"
#include "ftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock.h>
#include <direct.h>
#include <conio.h>

// Changed class name to CFtp because VC++ INSISTED 
// on thinking ftp:: was some kind of internet URL !

CFtp::CFtp() 
{
	Connected = 0;
	hListenSocket = INVALID_SOCKET;
	hControlSocket = INVALID_SOCKET;
	hDataSocket = INVALID_SOCKET;
	bSendPort = 1;

	bMode = BINARY;

	m_strLocalDirectory = "";

	//InitWinsock();
};

CFtp::~CFtp() 
{
};

/*
 * Open
 * this function is called when the open command is issued.
 * it connects to the requested host and logs the user in.
 *
 */
int CFtp::Open(char *command)
{
	char *szHost=NULL;  /* remote host */
	int responseCode=0;

	/* do not do anything if we are already connected. */
	if (Connected) 
	{
		return SERVICE_READY; // 220 Service ready for new user. 
	}

	//if(!strncmp(command,"open ",5))
	//	szHost = strdup(&command[5]);
	//else if(!strncmp(command,"o ",2) )
	//	szHost = strdup(&command[2]);
	//else
	//	szHost = strdup(command);

	//hControlSocket = ConnectToServer(szHost, "21");
	hControlSocket = ConnectToServer(command, "21");

	Sleep(1);
	responseCode=hControlSocket;

	if(hControlSocket > 0)
	{
		Connected = 1;         /* we are now connected */
		responseCode = GetReply(); /* get reply (welcome message) from server */
	}
	//free(szHost); /* free the strdupped string */
	return responseCode;
}
 
/*
 * Login
 * this function logs the user into the remote host.
 * prompts for username and password
 * or parses username and password from command (user <username>:<password>)
 */
int CFtp::Login(char *szUser, char * szPassword)
{
	int responseCode = 0;
	if(Connected)  
	{
		/* send user name & password to server  & get reply message */
		sprintf(szBuffer,"USER %s\r\n", szUser);
		SendControlMsg(szBuffer);
		responseCode = GetReply();

		if (USER_NAME_OKAY == responseCode) // Now send password. 
		{
			sprintf(szBuffer,"PASS %s\r\n", szPassword);
			SendControlMsg(szBuffer);
			responseCode = GetReply();
		}
	}
	return responseCode;
}

/* Closes connection to the ftp server. */
void CFtp::Close(void)
{
	if (Connected)
	{
		SendControlMsg("quit\r\n");
		GetReply();
		CloseControlConnection();
		hControlSocket = -1;
		Connected = 0;
	}
}

int CFtp::Delete(char *command)
{
	if(!Connected || (GetListenSocket() < 0)) 
		return 0;

	this->Lock();
	sprintf(szBuffer,"DELE %s\r\n", command);
	SendControlMsg(szBuffer);

	int retCode = GetReply();
	this->Unlock();
	return retCode;
}

int CFtp::RMDir(char *command)
{ 
	if(!Connected || (GetListenSocket() < 0)) 
		return 0;

	this->Lock();

	sprintf(szBuffer,"RMD %s\r\n", command);
	SendControlMsg(szBuffer);

	int retCode = GetReply();
	this->Unlock();
	return retCode;
}

////// SOMETHING ABOUT using these causes a memory leak !!!!
void CFtp::Move(char *command)
{ 
	char *files= (char *)calloc(strlen(command)+1, 1);
	int len = strlen(command);
	strncpy(files,(const char *)command, len);
	char *origFileName = strtok(files," " );
	char *newFileName = strtok(NULL,"\n" );

	if (!Connected || (GetListenSocket() < 0)) 
		return;

	sprintf(szBuffer, "RNFR %s\r\n", origFileName);
	SendControlMsg(szBuffer);
	if (GetReply() == FILE_ACTION_PENDING) // 350 Requested file action pending further information. 
	{
		sprintf(szBuffer,"RNTO %s\r\n", newFileName);
		MoveTo(szBuffer);
	}
}

void CFtp::MoveTo(char *command)
{ 
	char szBuffer[64]={0};

	if(!Connected ) 
		return;

	sprintf(szBuffer,"%s\r\n", command);
	SendControlMsg(szBuffer);
	GetReply();
}

///////////////////////////////////
// Get Remote Directory listing. //
///////////////////////////////////
void CFtp::RemoteList()
{
	char newbuffer[1025];
	if (!Connected || (GetListenSocket() < 0)) 
		return;

	int nOldMode = bMode;
	Ascii();

	strcpy(szBuffer, "LIST -l\r\n");

	/* send command to server and get response */
	SendControlMsg(szBuffer);
	memset(szBuffer, 0, 1024);
	int nCode = GetReply();

	/* accept server's connection */
	if (AcceptConnection() < 0) 
		return;

	memset(newbuffer, 0, 1025);
	m_strDirListing.clear();
	while (ReadDataMsg(newbuffer, 1024) > 0) 
	{
		m_strDirListing.append(newbuffer);
		memset(newbuffer, 0, 1025);
	}

	//// WHY does he close the connection ?!?!?!?!?!
	/////// I don't get it !!!!
	CloseDataConnection(hDataSocket);
	GetReply(); // Why is this call needed ? (can't change dirs without it ...)

	if (nOldMode == BINARY) // Restore to Binary mode
		Binary();
}

/*
 * CD
 * change to another directory on the remote system
 */
int CFtp::RemoteChangeDir(char *command)
{
	int responseCode=0;
	if(Connected)
	{
		sprintf(szBuffer, "CWD %s\r\n", command);
		SendControlMsg(szBuffer);
		responseCode = GetReply();
	}
	return responseCode;
}

/*
 * Binary
 * set file transfer mode to binary
 */
void CFtp::Binary()
{
	if (Connected ) 
	{
		sprintf(szBuffer, "TYPE I\r\n");
		SendControlMsg(szBuffer);
		GetReply();
		bMode = BINARY;
	}
}

/*
 * Ascii
 * set file transfer mode to ascii text
 */
void CFtp::Ascii()
{
	if (Connected ) 
	{
		sprintf(szBuffer, "TYPE A\r\n");
		SendControlMsg(szBuffer);
		GetReply();
		bMode = ASCII;
	}
}

int CFtp::GetSYST()
{
	int retCode = 0;
	if (Connected ) 
	{
		SendControlMsg("SYST\r\n");
		retCode = GetReply();
	}
	return retCode;
}

/*
 * Rhelp
 * sends a help command to the server.
 */
void CFtp::Rhelp(char *command )
{
	if (Connected)
	{
		char *szCommand;

		strtok(command," ");
		szCommand=strtok((char *)NULL, " ");

		if(szCommand && *szCommand)
			sprintf(szBuffer,"HELP %s\r\n", szCommand);
		else 
			sprintf(szBuffer, "HELP\r\n");

		SendControlMsg(szBuffer);
		GetReply();
	}
}

/*
 * retrieves the current directory on the remote host
 */
void CFtp::PWD()
{
	if (Connected ) 
	{
		sprintf(szBuffer, "PWD\r\n");
		SendControlMsg(szBuffer);
		GetReply();
	}
}

int CFtp::GetListenSocket()
{
    int sockfd, flag=1,len;
    struct sockaddr_in  serv_addr, TempAddr;
    char *port,*ipaddr;
    char szBuffer[64]={0};

    // Open a TCP socket (an Internet stream socket).
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		return INVALID_SOCKET;

	if (bSendPort) // Fill in structure fields for binding 
	{
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family      = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port        = htons(0); /* let system choose */
	}
	else // reuse the control socket then.
	{
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag)) < 0) 
		{
			closesocket(sockfd);
			return INVALID_SOCKET;
		}
	}

	////////////////////////////////////
    // bind the address to the socket //
	////////////////////////////////////
    if (bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
        closesocket(sockfd);
		return INVALID_SOCKET;
    }

    len = sizeof(serv_addr);

    if (getsockname(sockfd, (struct sockaddr *)&serv_addr, &len) < 0)
	{
		closesocket(sockfd);
		return INVALID_SOCKET;
    }

    len = sizeof(TempAddr);

	if (getsockname(hControlSocket, (struct sockaddr *)&TempAddr, &len)<0)
	{
		closesocket(sockfd);
		return INVALID_SOCKET;
	}
	ipaddr = (char *)&TempAddr.sin_addr;
	port  = (char *)&serv_addr.sin_port;

#define  UC(b)  (((int)b)&0xff)

	sprintf(szBuffer,"PORT %d,%d,%d,%d,%d,%d\r\n",
		UC(ipaddr[0]), UC(ipaddr[1]), UC(ipaddr[2]), UC(ipaddr[3]),
		UC(port[0]), UC(port[1]));

	/////////////////////////////////////////////////////
    // allow ftp server to connect to only one server. //
	/////////////////////////////////////////////////////
    if (listen(sockfd, 1) < 0) 
	{
        closesocket(sockfd);
		return INVALID_SOCKET;
    }
    SendControlMsg(szBuffer);
    GetReply();
    hListenSocket = sockfd;
    return sockfd;
}

int CFtp::AcceptConnection()
{
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    int sockfd;

    sockfd = accept(hListenSocket, (struct sockaddr *) &cli_addr, &clilen);

    if (sockfd < 0) 
	{
		return INVALID_SOCKET;
    }

    hDataSocket = sockfd;
    closesocket(hListenSocket);
    return sockfd;
}

int CFtp::ConnectToServer(char *name, char *port)
{
	int s;
	unsigned int portnum;
	struct sockaddr_in server;
	struct hostent *hp;

	while(name && *name == ' ') name++;

	if(!name || ! (*name))
		return INVALID_SOCKET;

	portnum = atoi(port);
	bzero((char *) &server, sizeof(server));

	if (isdigit(name[0])) 
	{
		server.sin_family      = AF_INET;
		server.sin_addr.s_addr = inet_addr(name); // Converts dot address to integer.
		server.sin_port        = htons(portnum);
	}
	else
	{ 
		if ((hp = gethostbyname(name)) == NULL)
		{
			return INVALID_SOCKET;
		}

		bcopy(hp->h_addr, (char *) &server.sin_addr, hp->h_length);
		server.sin_family = hp->h_addrtype;
		server.sin_port = htons(portnum);  
	}

	///////////////////
	// create socket //
	///////////////////
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 1) 
	{
		return INVALID_SOCKET;
	}

	if (connect(s,(struct sockaddr *)&server, sizeof(server))< 0) 
	{
		return INVALID_SOCKET;
	}
	setsockopt(s, SOL_SOCKET, SO_LINGER,    0, 0);
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, 0, 0);
	setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, 0, 0);
	hDataSocket = s;
	return s;
}

// This is only called ONCE in the whole project!!!!
// do we need it ?????
////////////////////////////////////////
int CFtp::ReadControlMsg(char *szBuffer, int len)
{
	int ret = recv(hControlSocket, szBuffer, len, 0);
	if (ret < 0)
		ret = 0;
	return ret;
}

int CFtp::SendControlMsg(char *szBuffer)
{
	if (send(hControlSocket, szBuffer, strlen(szBuffer), 0) <= 0)
		return 0;
	else
		return 1;
}

// Only called twice in whole project ...
//////////////////////////////////////////////
int CFtp::ReadDataMsg(char *szBuffer, int len)
{
	int ret = recv(hDataSocket, szBuffer, len, 0);
	if (ret <= 0)
		ret = 0;
	return ret;
}

int CFtp::SendDataMsg(char *szBuffer, int len)
{
	if (send(hDataSocket, szBuffer, len, 0) <= 0)
		return 0;
	else
		return 1;
}

void CFtp::CloseDataConnection(int hDataSocket)
{
	closesocket(hDataSocket);
	hDataSocket = INVALID_SOCKET;
}

void CFtp::CloseControlConnection(void)
{
	closesocket(hControlSocket);
	hControlSocket = INVALID_SOCKET;
}

void CFtp::CloseListenSocket(void)
{
	closesocket(hListenSocket);
	hListenSocket = INVALID_SOCKET;
}

int CFtp::CheckControlMsg(char *szPtr, int len)
{
    return recv(hControlSocket, szPtr, len, MSG_PEEK);
}

int CFtp::GetLine()
{
	int done=0, iRetCode =0, iLen, iBuffLen=0;
	char *szPtr = szBuffer, nCode[3]={0},ch=0;

	while((iBuffLen < 1024) && (CheckControlMsg(&ch, 1) > 0))
	{
		iLen = ReadControlMsg(&ch, 1);
		iBuffLen += iLen;
		*szPtr = ch;
		szPtr += iLen;
		if(ch == '\n' )
			break;    // we have a line: return
	}

	*(szPtr+1) = (char)0;
	strncpy(nCode, szBuffer, 3);
	return (atoi(nCode));
}

int CFtp::GetReply()
{
	int done = 0, iRetCode = 0;
	char *token;
	memset(szBuffer, 0, 1024);

	while (!done ) 
	{
		iRetCode = GetLine();
		token = strtok(szBuffer, "\r\n"); // I think he does this to replace endline with NULL.
		if (szBuffer[3] != '-' && iRetCode > 0)
			done = 1;
	}
	return iRetCode;
}

int CFtp::InitWinsock()
{
	WSADATA WsaData;

	if (!WSAStartup(0x0101, &WsaData)) 
		return 1;
	else 
		return 0;
}

void CFtp::CleanUp()
{
	WSACleanup();
}

/*
 * GetFile
 * called to retrieve a file from remote host
 */
int CFtp::GetFile(char *fname)
{
	int responseCode = 0;
	if (Connected && *fname != NULL)
	{
		if (strlen(fname) > 0) // Ensure filename is valid.
		{
			FILE *fp = NULL;
			int fd, nTotal = 0, nBytesRead = 0, aborted = 0;
			char *abortstr = "ABOR\r\n";

			if (GetListenSocket() < 0) // obtain a listen socket
				return responseCode;

			//////////////////////////////////////////////
			// send command to server and read response //
			//////////////////////////////////////////////
			sprintf(szBuffer, "RETR %s\r\n", fname);
			if (SendControlMsg(szBuffer))
			{
				//return responseCode;
				responseCode = GetReply();
				if (responseCode == FILE_STATUS_OKAY) // 150 File status okay, about to open data connection. 
				{
					/////////////////////////////////////
					// open the file with current mode //
					/////////////////////////////////////
					string strFullName = m_strLocalDirectory + "\\" + fname;
					if(! (fp = fopen(strFullName.c_str(), (bMode == ASCII) ? "wt" : "wb"))) 
						return responseCode;

					//////////////////////////////
					// accept server connection //
					//////////////////////////////
					if (AcceptConnection() <= 0) 
					{
						fclose(fp);
						return responseCode;
					}

					////////////////////////////
					// now get file and store //
					////////////////////////////
					fd = fileno(fp);

					while ((nBytesRead = ReadDataMsg(szBuffer, 1024)) > 0)
					{
						write (fd, szBuffer, nBytesRead);
						nTotal += nBytesRead;
					}

					close(fd);
					fclose(fp);
					CloseDataConnection(hDataSocket);
					responseCode = GetReply();
				}
			}
		}
	}
	return responseCode;
}

/*
 * PutFile
 * called to transfer a file to the remote host using the current
 * file transfer mode.  it's just like GetFile.
 */
int CFtp::PutFile(char *fname)
{
	int retcode = 0;
	if (Connected && *fname != NULL)
	{
		if (strlen(fname) > 0)
		{
			FILE *fp = NULL;
			string strFullName = m_strLocalDirectory + "\\" + fname;

			if (!(fp = fopen (strFullName.c_str(), (bMode == ASCII) ? "rt" : "rb"))) 
			{
				return retcode;
			}

			if (GetListenSocket() < 0) 
			{
				fclose(fp);
				return retcode;
			}
		   
			/////////////////////////////////////////
			// send command to server & read reply //
			/////////////////////////////////////////
			sprintf(szBuffer, "STOR %s\r\n", fname);
			if (!SendControlMsg(szBuffer)) 
			{
				fclose(fp);
				return retcode;
			}

			retcode = GetReply();
			if (retcode != FILE_STATUS_OKAY)
				return retcode;

			//////////////////////////////
			// accept server connection //
			//////////////////////////////
			if (AcceptConnection() <= 0) 
			{
				fclose(fp);
				return retcode;
			}

			///////////////////
			// now send file //
			///////////////////
			int fd, nTotal = 0, nBytesRead = 0;
			fd = fileno(fp);
			while ((nBytesRead = read(fd, szBuffer, 1024)) > 0) 
			{
				SendDataMsg(szBuffer, nBytesRead);
				nTotal += nBytesRead;
			}

			///////////////////////////
			// close data connection //
			///////////////////////////
			CloseDataConnection(hDataSocket);
			close(fd);
			fclose(fp);
			retcode = GetReply();
		}
	}
	return retcode;
}

////////////////////////////////////////////
// Sends a NOOP (no operation) cmd to FTP //
// server to keep the connection alive.   //
////////////////////////////////////////////
int CFtp::KeepAlive()
{	int retCode = 0;
	if (Connected)
	{
		this->Lock();
		sprintf(szBuffer, "NOOP\r\n");
		SendControlMsg(szBuffer);
		retCode = GetReply();
		this->Unlock();
	}
	return retCode;
}

///////////////////////////
// Mkdir on remote host. //
///////////////////////////
int CFtp::MakeDir(char *command)
{ 
	int responseCode = 0;

	if (!Connected)
		responseCode = 0;
	else if (GetListenSocket() < 0)
		responseCode = 0;
	else
	{
		sprintf(szBuffer,"MKD %s\r\n", command);
		SendControlMsg(szBuffer);
		responseCode = GetReply();
	}
	return responseCode;
}

// REMOTE change dir up
int CFtp::CDUP()
{
	int retCode = 0;
	if (Connected)
	{
		sprintf(szBuffer, "CDUP\r\n");
		SendControlMsg(szBuffer);
		retCode = GetReply();
	}
	return retCode;
}
