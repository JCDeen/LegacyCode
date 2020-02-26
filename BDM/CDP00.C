/************************************************************************/
/*                              CDP00.C                                 */
/************************************************************************/

/************************************************************************/
/* PURPOSE:    General support file for CDP.C .                         */
/* AUTHOR(S):  James C. Deen, Jr.                                       */
/* LOCATION:   BDM, Huntsville, Alabama                                 */
/* MAINTENANCE:                                                         */
/*       Created on Oct. 3, 1991                                        */
/************************************************************************/

#include "cdp.h"

/************************************************************************/
/*                       General Utility Packages                       */
/************************************************************************/

/*******************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                   */
/* PURPOSE  : Center justify a message string and display it on    */
/*            line of main_menu window's data area.                */
/* ALGORITHM: Use C-Tools Plus stpjust() function.                 */
/* INPUT    : buffer - message string to justify and display.      */
/* OUTPUT   : Video monitor .                                      */
/*******************************************************************/
void status_line(char *buffer)
{  char msg[79];
   stpjust(msg, buffer, ' ', 78, JUST_CENTER);
   vidspmsg(23, 1, BLUE, WHITE, msg);
}

/*******************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                   */
/* PURPOSE  : Inform users who authored this brilliant code.       */
/* ALGORITHM: Saves & restores previous screen. Returns to main()  */
/*       after a keystroke, or 7 seconds, whichever comes first.   */
/* INPUT    : None.                                                */
/* OUTPUT   : monitor.                                             */
/*******************************************************************/
void Program_ID_Screen(void)
{
   BWINDOW *pwin;
   BORDER  bord;
   WHERE   loc;
   int     mode, columns, act_page, cursor_was_off, srow, scol, high, low;
   time_t  t0, t1;

   assert((pwin = wncreate(23, 78, utnybbyt(WHITE, BLUE))) != NIL);
   assert (!b_wnerr);

   /* Determine the active page of current display device. */

   loc.dev	= scmode(&mode,&columns,&act_page);
   loc.page	= act_page;
   loc.corner.row = 1;
   loc.corner.col = 1;

   /* Save former cursor position and size. Display Program ID Window. */

   cursor_was_off = sccurst(&srow,&scol,&high,&low);

   bord.type = BBRD_DDDD;
   bord.attr = utnybbyt(WHITE, BLUE);
   assert( wndsplay(pwin, &loc, &bord) != NIL);

   vidspmsg( 6, 26, BLUE, WHITE, "Computer Diagnostic Program");
   vidspmsg(11, 26, BLUE, WHITE, "        Programmers:       ");
   vidspmsg(13, 26, BLUE, WHITE, "     James C. Deen, Jr.    ");
   vidspmsg(14, 26, BLUE, WHITE, "        Lisa Pereira       ");
   vidspmsg(19, 26, BLUE, WHITE, " (c) 1991 BDM International");

   /********************************************************/
   /* Return to main() after 7 seconds - OR - a keystroke. */
   /********************************************************/
   kbflush();
   time(&t0);
   do
      time(&t1);
   while (!kbhit() && (difftime(t1, t0) < 7.0));
   kbflush();

   /****************************/
   /* Restore previous screen. */
   /****************************/
   assert(wnremove(pwin) != NIL);
   assert(wndstroy(pwin) == WN_NO_ERROR);
   sccurset(srow,scol);
   scpgcur(cursor_was_off,high,low,CUR_NO_ADJUST);
}

/*******************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                   */
/* PURPOSE  : Send a message to log file and/or com port.          */
/* ALGORITHM: Accesses global variables to determine destinations. */
/*            Pad strings < MIN_LENGTH bytes with @ least one 'P'. */
/* INPUT    : message - character string to be transmitted.        */
/* OUTPUT   : one or more of the following:                        */
/*       TRACKCOM.DAT - file to track serial output for debugging. */
/*       com1/com2 - the designated serial port (only 1 allowed).  */ 
/*******************************************************************/
#define MIN_LENGTH 9
void report(char *message)
{  int i;
   char buffer[MIN_LENGTH+1];

   if (!calibration_run)
   {  strcpy(buffer, message);
      while(strlen(buffer)<MIN_LENGTH) strcat(buffer, "P");

      for (i=0; i<strlen(buffer); i++)
         sio_put(buffer[i]);

      if (menu_map[TRACK_COMOUT].status_box == ENGAGED)
         fprintf(trackcom_file, "%s\n", buffer);
   }
}

/************************************************************************/
/*                 Async (comm port) support packages.                  */
/************************************************************************/

#include "async.h"         /* Formerly SERIO.H "Dos 5 : A Developers Guide" */

static struct 
{  char *text;             /* <- Menu selection labels.                   */
   char *pkeys;            /* <- Fast access keys.                        */
   int  baud_rate_index;   /* <- Parameters for sio_setup() function from */
   int  parity;            /* <- "Dos 5 : A Developers Guide", by Al      */
   int  data_bits;         /* <- Williams.                                */
   int  stop_bits;         /* <-                                          */
} comm_menu_map[] =
{ { "9600 None Seven One", "96Nn", 6, 0, 7, 1 },
  { "9600 Even Seven One", "96Ee", 6, 2, 7, 1 },
  { "9600 Odd  Seven One", "96Oo", 6, 1, 7, 1 },
  { "4800 None Seven One", "48Nn", 5, 0, 7, 1 },
  { "4800 Even Seven One", "48Ee", 5, 2, 7, 1 },
  { "4800 Odd  Seven One", "48Oo", 5, 1, 7, 1 },
  { "2400 None Seven One", "2Nn" , 4, 0, 7, 1 },
  { "2400 Even Seven One", "2Ee" , 4, 2, 7, 1 },
  { "2400 Odd  Seven One", "2Oo" , 4, 1, 7, 1 },
  { "1200 None Seven One", "1Nn" , 3, 0, 7, 1 },
  { "1200 Even Seven One", "1Ee" , 3, 2, 7, 1 },
  { "1200 Odd  Seven One", "1Oo" , 3, 1, 7, 1 }
};

/******************************************************************/
/* INPUT: port_no - 1 for COM1, etc.                              */
/******************************************************************/
int comm_port_menu(int port_no)
{  static int row[2] = {7,7};
   BMENU  *comm_menu;
   BORDER bord;
   WHERE  loc;
   char   t_buffer[30];
   int    mode, columns, act_page, cursor_was_off, srow, scol, high, low;
   int    col, ch, scancode;
   int    index, err_code;

   assert ((port_no >= 1) && (port_no <= 4));

   assert ((comm_menu = mncreate(sizeof(comm_menu_map) / sizeof(comm_menu_map[0]), 23,
                           utnybbyt(BLACK, MAGENTA),
		                     REVERSE,           /* Highlight bar.     */
		                     NORMAL,            /* Protected items.   */
		                     NORMAL )) != NIL); /* Item descriptions. */
   assert (!b_wnerr);

   bord.type   = BBRD_DDDD | BBRD_TCT;
   bord.attr   = utnybbyt(BLACK, MAGENTA);
   bord.ttattr = utnybbyt(BLACK, MAGENTA);

   sprintf(t_buffer, " Configure Comm Port %d ", port_no);
   bord.pttitle = t_buffer;

   for (index=0; index< (sizeof(comm_menu_map)/sizeof(comm_menu_map[0])); index++)
      assert ( mnitmkey(comm_menu, index, 2, MN_NOPROTECT, 
               comm_menu_map[index].text, comm_menu_map[index].pkeys,
               MN_SELECT) != NIL);

   loc.dev	= scmode(&mode,&columns,&act_page);
   loc.page	= act_page;
   loc.corner.row =  5;
   loc.corner.col = 30;

   cursor_was_off = sccurst(&srow,&scol,&high,&low);
   assert (mndsplay(comm_menu, &loc, &bord) != NIL);

   status_line("Select appropriate Comm Port Configuration, then ENTER");

   assert (mnread(comm_menu, row[port_no], col, &row[port_no], &col, &ch, &scancode,
               MN_UNKNOWN_BEEP | MN_KEEP_HIGHLIGHT ) == WN_NO_ERROR);

   #ifdef USE_ASYNC_C_INTERRUPTS
      sio_stop();             /* Clear/restore previously set interrupts. */
      err_code = sio_setup(port_no,         
                           comm_menu_map[row[port_no]].baud_rate_index,
                           comm_menu_map[row[port_no]].data_bits,
                           comm_menu_map[row[port_no]].parity,
                           comm_menu_map[row[port_no]].stop_bits);
      assert(err_code == 0);
   #endif

   assert(mndstroy(comm_menu) == WN_NO_ERROR);
}

/*************************************************************************/
/* PURPOSE: Polls the comm port and keyboard until a character from the  */ 
/*          quit_string is detected, returning that keystroke.           */
/*          If ESCAPE is detected at any time, the function returns.     */
/* INPUT  : quit_string - list of characters causing exit from function. */
/* OUTPUT : none.                                                        */
/* RETURNS: Uppercased (if appropriate) keystroke causing exit from loop.*/
/* NOTES  : The appropriate comm port should have been already set up.   */
/*************************************************************************/
char wait_for_char(char *quit_list)
{  char inkey = (char) 1;

   do
   {  
      if (sio_charready())             /* Check if character ready. */
         inkey = (char) sio_get();     /* Get char from comm port.  */
      else
         if (kbhit())                  /* Check if character ready. */
            inkey = getch();           /* Get char from keyboard.   */
                          
   } while ((strchr(quit_list, inkey )==NULL) && (inkey != (char) 27));
   kbflush();
   return(toupper(inkey));
}


/*************************************************************************/
/* PURPOSE: Similar to Microsoft's difftime(), except returns resolution */ 
/*          of about .054 seconds vs. difftime()'s 1 sec resolution.     */
/* ALGORITHM: Computes values based on fact that time of day counter     */
/*          tics about 18.2 times per second.                            */
/* INPUT  : t1, t0 - long values obtained via _bios_timeofday() calls.   */
/* OUTPUT : none.                                                        */
/* RETURNS: Difference of t1-t0 in seconds.                              */
/*************************************************************************/
double difftimer(long t1, long t0)
{  return (((double)(t1-t0))/(1573040./86400.));
}

#ifdef COMPILE_TOBINSTR
/***************************************************************************/
/* PURPOSE: Generate a string containing binary representation of input k. */
/* ALGORITHM: Fills string with MSB at left, LSB at right.                 */
/* INPUT  : k - value to "convert" .                                       */
/*    ndigits - number of binary digits to convert.                        */
/* RETURNS: Pointer to string containing binary representation of k.       */
/***************************************************************************/
char *tobinstr(unsigned k, int ndigits)
{  static char buffer[17];
   int i;
   assert(ndigits>=0 && ndigits<=16);
   for (i=ndigits-1; i>=0; i--) buffer[ndigits-1-i] = (k>>i)&1 ? '1' : '0';
   buffer[ndigits] = '\0';
   return(buffer);
}
#endif

/*************************************************************************/
/* PURPOSE: Write formatted output to test_window and log_file.          */
/* ALGORITHM: Functions just like printf().                              */
/* INPUT  : Variable length argument list.                               */
/* OUTPUT : test_window, log_file.                                       */
/* RETURNS: number of chars written if successful, negative value if err.*/
/*************************************************************************/
#define TL_BUFF_SIZE 81
int tlprintf (char *fmt, ...)
{  va_list arg_ptr;
   int result;
   char buffer[TL_BUFF_SIZE];
	/* Point arg_ptr to the parameter just before the */
   /* beginning of the variable argument list.     */

   va_start (arg_ptr, fmt);

   /*******************************************************************/
	/* Put a NUL at the end of the vsprintf() buffer to flag overflow. */
   /* Interpret format string and place into buffer[].                */
   /* Discard the variable argument list.                             */
   /*******************************************************************/
   buffer[TL_BUFF_SIZE-1] = '\0';
   result = vsprintf(buffer, fmt, arg_ptr);
   va_end (arg_ptr); 

   if (calibration_run==TRUE) return(0);

   assert(buffer[TL_BUFF_SIZE-1] == '\0');      /* Overflow not allowed! */

   if (result > 0)
   {  wnselect(test_window);
      wnprintf("%s\n", buffer);
      if (menu_map[OUTPUT_TO_LOG_FILE].status_box==ENGAGED)
         fprintf(log_file, "%s\n", buffer);
   }
   return (result);
}

/*************************************************************************/
/* PURPOSE: Formatted version of report().                               */ 
/* ALGORITHM: Functions just like printf().                              */
/* INPUT  : Variable length argument list.                               */
/* OUTPUT : To the function report().                                    */
/* RETURNS: number of chars written if successful, negative value if err.*/
/*************************************************************************/
#define R_BUFF_SIZE 81
int reportf (char *fmt, ...)
{  
   va_list arg_ptr;
   int result;
   char buffer[R_BUFF_SIZE];

   if(calibration_run) return(0);

   va_start (arg_ptr, fmt);
   buffer[R_BUFF_SIZE-1] = '\0';
   result = vsprintf(buffer, fmt, arg_ptr);
   va_end (arg_ptr); 
   assert(buffer[R_BUFF_SIZE-1] == '\0');    /* Enforce no overflow.  */
   assert(result>0);                         /* Ensure chars printed. */
   report(buffer);                           /* Serial I/O.           */
   return (result);
}
#undef R_BUFF_SIZE
