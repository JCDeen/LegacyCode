/************************************************/
/*                   CDP.H                      */
/************************************************/

/************************************************/
/* PURPOSE:    Support file for CDP.C .         */
/* AUTHOR:     James C. Deen, Jr.               */
/* LOCATION:   BDM, Huntsville, Alabama         */
/* MAINTENANCE:                                 */
/*       Creation:   Sept. 25, 1991             */
/************************************************/

#include "blaise.h"     /* C-TOOL Plus 5.0 */

#include <assert.h>     /* Microsoft C 5.0 */

#include <bios.h>       /* Microsoft C 5.0 */
#include <conio.h>      /* Microsoft C 5.0 */
#include <ctype.h>      /* Microsoft C 5.0 */
#include <dos.h>        /* Microsoft C 5.0 */
#include <errno.h>      /* Microsoft C 5.0 */
#include <fcntl.h>      /* Microsoft C 5.0 */
#include <graph.h>      /* Microsoft C 5.0 */
#include <process.h>    /* Microsoft C 5.0 */
#include <stdarg.h>
#include <stdio.h>      /* Microsoft C 5.0 */
#include <stdlib.h>     /* Microsoft C 5.0 */
#include <string.h>     /* Microsoft C 5.0 */
#include <time.h>       /* Microsoft C 5.0 */
#include <malloc.h>     /* Microsoft C 5.0, added on 28 OCT 91 */

/*********************************************************/
/*           Function prototype declarations.            */
/*********************************************************/
/*            CDP00.C             */
/**********************************/
extern void status_line(char *buffer);
extern void report(char *message);
extern double not_avail(int);
extern double no_func(int fno);
extern void bogus(void);
extern void Program_ID_Screen(void);
extern int  comm_port_menu(int port_no);
extern void puts_com(char *string);
extern char wait_for_char(char *quit_list);
extern double difftimer(long t1, long t0);
extern int tlprintf (char *fmt,...);
extern int reportf (char *fmt, ...);
extern char *tobinstr(unsigned k, int ndigits);

/**********************************/
/*            CDP01.C             */
/**********************************/
extern double equip_list(int dummy);
extern double read_kb_flags(int dummy);
extern double cport_status(int port_no);
extern double test_cmd_com(int dummy);
extern double hware_int_map(int dummy);
extern double timer_8253(int dummy);
extern double math_80x87 (int dummy);

/**********************************/
/*            CDP02.C             */
/**********************************/
extern double disk_check(int drive_id);
extern double disk_to_disk_check (int drives_combo);
extern double printer_test(int moron);
extern double test_video(int x);
extern double env_test(int x);
extern double heap_test(int x);

/*********************************************************/
/*          Preprocessor macros/definitions.             */
/*********************************************************/

#define ETHERNET
#ifdef ETHERNET
   #define FD "E:"
#else
   #define FD "C:"
#endif


#undef NDEBUG              /* Enable assert() macros. */

typedef int BOOLEAN;
#define FALSE 0
#define TRUE  1
#define DISENGAGED FALSE
#define ENGAGED    TRUE

/***************************************************************/
/* Some typing shortcut macros to access Test Control Options. */
/***************************************************************/
#define r_to_com      menu_map[REPORT_TO_COM].status_box
#define repeat_tests  menu_map[REPEAT_UNTIL_ESC].status_box

/************************************************************/
/*          Enumerate menu item selection indices.          */
/************************************************************/
extern enum MENU_INDICES
{ VIDEO=0, READ_KB_FLAGS, COMM1_TEST, COMM2_TEST, LINE_PRINTER_TEST,
  EQUIPMENT_LIST, TIMER_TEST, HWARE_INTRPT_ADDRESSES, DOS_ENVIRONMENT_SPACE,
  CMD_COM_INTEGRITY, COPROCESSOR_CHECK, HEAP_CHECK, FLOPPY_DRIVE_A,
  FLOPPY_DRIVE_B, HARD_DRIVE_C, DRIVES_A_B, DRIVES_A_C, DRIVES_B_C,
  OUTPUT_TO_LOG_FILE, TRACK_COMOUT, CONFIGURE_COM_1, CONFIGURE_COM_2,
  EXECUTE_TESTS, EXIT_PROGRAM, VIEW_OUTPUT, VIEW_EXEC_TIMES, NULL_SELECTION };

#define ID_MASK         31  /* ddd11111 - AND with MENU_INDICES above. */
#define CMD_MASK        96  /* d1100000 - AND with x_CONTROL below   . */

#define OPTION_CONTROL  32  /* d0100000 - indicates a test control option. */
#define PROGRAM_CONTROL 64  /* d1000000 - indicates a command to program.  */
/****************************************************************/
/*                    Structure definitions.                    */
/****************************************************************/

struct MENU_MAP
{  double (*pf)();      /* Pointer to a corresponding function. */
   int iarg;            /* Integer argument to be used with fct.*/
   int row;             /* Display row.                         */
   int col;             /* Display column.                      */
   BOOLEAN status_box;  /* Indicates function turned ON or OFF. */
   char *text;          /* "Read Keyboard Flags", etc.          */
   char *pkeys;         /* Fast access keys to menu items.      */
   int  item_id;        /* menu_map[] index | item type.        */
};

/****************************************************************/
/*                       Global variables                       */
/*                                                              */
/* NOTE: 'extern' not needed to reference these declarations in */
/*       the various source files. See Microsoft Language Ref-  */
/*       erence manual, page 84. This method is NOT recommended */
/*       for library resident code.                             */
/****************************************************************/
struct   MENU_MAP menu_map[];
FILE     *log_file, *trackcom_file;
time_t   t_start, t_stop;

int act_page, global_device;
int video_time, numofstringlines;

BWINDOW  *test_window;        /* Echo test-in-progress info to this window. */
BOOLEAN  reset_status_bytes;
BOOLEAN  calibration_run;

#define USE_ASYNC_C_INTERRUPTS
