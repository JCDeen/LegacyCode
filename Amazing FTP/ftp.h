#ifndef __CFtp__
#define __CFtp__

#include <io.h>
#include <string>
#include "afxmt.h"
using namespace std;

#define DEFAULT_PORT_NUM 21
#define PASSWORD_LENGTH 256
//#define EINTR WSAEINTR 

#define bzero(x,y) memset(x,0,y)
#define bcopy(x,y,z) memcpy(y,x,z)
#define close closesocket

enum 
{	
	LS = 0, BINARY, ASCII, PWD, CD, OPEN, CLOSE, QUIT, LCD, LLS, LDIR, 
	USER, SHELL, IGNORE_COMMAND, GET, PUT, HELP, RHELP,
	FTP_COMPLETE=1, FTP_CONTINUE, FTP_ERROR
};

class CFtp : public CCriticalSection
{
public:
	
	CFtp();
	~CFtp();

	int  Open(char *);
	void RemoteList();
	int  RemoteChangeDir(char *);
	//void ShellCommand(char *);
	int Login(char *szUser, char * szPassword);

	void Close(void);
	//void LocalChangeDir( char *);
	//void LLS(char * );
	void Binary();
	void Rhelp(char *);
	void Ascii();
	int GetSYST();
	void PWD();
	int Delete(char *);
	int RMDir(char *command); // For Unix side only.

	void Move(char *);
	void MoveTo(char *);
	int  CheckFds(char *);
	int  MakeDir(char *command);
	int CDUP();

	int GetFile(char *fname);
	int PutFile(char *fname);

	int KeepAlive();

	char szBuffer[1025];		// Buffer used to read/write.
	string m_strLocalDirectory;	// Where to get/put file from/to on local machine.
	string m_strDirListing;		// Holds list of files of Remote dir as string.

	static const int FILE_STATUS_OKAY = 150;
	static const int COMMAND_OKAY = 200;
	static const int SERVICE_READY = 220;
	static const int FILE_ACTION_SUCCESSFUL = 226;
	static const int USER_LOGGED_IN_PROCEED = 230;
	static const int FILE_ACTION_COMPLETED = 250;
	static const int PATHNAME_CREATED = 257;
	static const int FILE_ACTION_PENDING = 350;
	static const int USER_NAME_OKAY = 331;
	static const int ACTION_NOT_TAKEN = 550;

	time_t m_tStart;
	//void //SetLastTimeUsed();

private:
	char szUser[20];    /* stores username */
	char szPass[256];	/* stores user password */
	int  Connected;     /* flag for connect status */
	int	m_nBusy;
	
	int hListenSocket;
	int hControlSocket;
	int hDataSocket;	
	int bSendPort;
	int bMode;

	int GetReply();
    int GetLine();
	void CleanUp();
	int SendControlMsg(char *); //, int);
	int SendDataMsg(char *szBuffer, int len);

	int ConnectToServer(char *name, char *port);
	int GetListenSocket();
	int InitWinsock();
	int AcceptConnection();
	void CloseControlConnection( void );
	void CloseDataConnection(int hDataSocket );
	void CloseListenSocket();
	int ReadDataMsg(char *szBuffer, int len);
	
	int ReadControlMsg(char *szBuffer, int len);
	int CheckControlMsg(char *szPtr, int len);

	//int CheckInput();
};

#endif