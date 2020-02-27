#ifndef PORTABLE_INCLIDED
#define PORTABLE_INCLIDED

//
// Portable.h: Includes some of the portability related issues..
//
#include <afxwin.h>
#include <afxsock.h>

typedef int			Int_t;
typedef DWORD		ChildID_t;

#define ioctl(x, y, z)	ioctlsocket(x, y, z)
#define sleep(x)		Sleep(1000*x)
#define WaferExit(x)	ExitThread(x)

#endif
