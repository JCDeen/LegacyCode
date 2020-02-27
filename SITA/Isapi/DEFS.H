#ifndef WAFERDEFS_INCLUDED
#define WAFERDEFS_INCLUDED

//
// Defs.h: This file defines the various constants that are used through out
// project Wafer
//

//
// Set the default timeouts for the packets. All timeouts are in seconds.
//
#define WIP_HEADER_TIMEOUT		60
#define WIP_REQUEST_TIMEOUT		60
#define WIP_RESPONSE_TIMEOUT	300

#define MAX_CLIENTID_LEN		12
#define MAX_USERID_LEN			20
#define MAX_PASSWORD_LEN		20
#define MAX_SYSTEM_LEN			20
#define MAX_IPADDR_LEN			15

#define MAX_ORIGIN_LEN			20
#define MAX_DESTN_LEN			20
#define MAX_CARRIER_LEN			03

#define MAX_TRANSACTION_LEN		1024

#endif
