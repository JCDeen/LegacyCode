#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#define CONFIG_FILE_NAME "WFServer.INI"

// ------------------------------------------------------------------
//
// Structure that maintains some of the configuration information
// required by the Wafer server. The data is read directly from a
// ".ini" file, that contains the required fields in the same order.
//
// 1. nListenPort - The port number at which the Wafer server listens
//                  to.
// 2. lBeginPID - The wafer server looks for PIDs in the range of
//                lBeginPID to lEndPID. lBeginPID is the starting PID
// 3. lEndPID   - The ending PID number. The start and the end PID 
//                could be swapped and the Wafer server may still work
//                since, the PID Generator that uses this entry takes
//                care of it.
// 4. lTimeout  - Timeout value in number of minutes
// 5. szTransServerAddress - This is the address of the mainframe DCP
//                           or the address of a Transaction server
// 6. nTransServerPort - The port number of the mainframe DCP or the
//                       port number of a Transaction server
// -------------------------------------------------------------------
struct WaferConfig {
	int		nListenPort;
	long	lBeginPID;
	long	lEndPID;
	long	lTimeout;
	char	szTransServerAddress[255];
	int		nTransServerPort;
};

#endif
