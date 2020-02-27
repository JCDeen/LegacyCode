#ifndef PORTABLE_INCLIDED
#define PORTABLE_INCLIDED

//
// Portable.h: Includes some of the portability related issues..
//
typedef int Int_t;

#ifdef _WIN32

#include <afxwin.h>
#include <afxsock.h>

#define ioctl(x, y, z)	ioctlsocket(x, y, z)
#define sleep(x)		Sleep(1000*x)
#define WaferExit(x)	ExitThread(x)

typedef HANDLE ChildID_t;

#else

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define closesocket(x)	close(x)
#define WaferExit(x)	exit(x)

typedef pid_t			ChildID_t;
typedef unsigned int	UINT;
typedef void			*LPVOID;
typedef int				SOCKET;
typedef int				BOOL;
typedef unsigned long	DWORD;

#define FALSE	0
#define TRUE	1

#define WINAPI

#endif

#endif
