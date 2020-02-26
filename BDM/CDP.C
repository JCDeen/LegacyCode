/***************************************************************************/
/*                                   CDP.C                                 */
/*                                                                         */
/*                        Computer Diagnostic Program                      */
/***************************************************************************/

/************************************************************************/
/* PURPOSE:    Main program for CDP.C .                                 */
/*                                                                      */
/*    CDP (Computer Diagnostic Program) is intended to exercise various */
/*    PC functions & devices, and track the deviations from the         */
/*    expected results.                                                 */
/*                                                                      */
/* AUTHOR(S):  James C. Deen, Jr.                                       */
/*             Lisa Pereira                                             */
/* LOCATION:   BDM, Huntsville, Alabama                                 */
/*                                                                      */
/* MAINTENANCE:                                                         */
/*    Created on Sept. 25, 1991                                         */
/*    Sept 28, 1992                                                     */
/*		Added preprocessor variable to use E: as fixed disk     */
/*		instead of C: .                                         */
/************************************************************************/

#include "cdp.h"

/*   ÚÄ fct. name  row Ä¿         */
/*   ³ fct. parm ÄÄÄ¿   ³   Ú col */
                                                
extern struct MENU_MAP menu_map[] =
{  { test_video   , 0,  3,  6, DISENGAGED, "Video Test         "  , "VvTt"    , VIDEO                },
   { read_kb_flags, 0,  4,  6, DISENGAGED, "Read Keyboard Flags"  , "RrKkFf"  , READ_KB_FLAGS        },
   { cport_status , 1,  5,  6, DISENGAGED, "Comm Port 1 Status "  , "CcPp1Ss" , COMM1_TEST           },
   { cport_status , 2,  6,  6, DISENGAGED, "Comm Port 2 Status "  , "CcPp2Ss" , COMM2_TEST           },
   { printer_test , 0,  7,  6, DISENGAGED, "Line Printer Test  "  , "LlPpTt"  , LINE_PRINTER_TEST    },
   { equip_list   , 0,  8,  6, DISENGAGED, "Equipment List     "  , "EeLl"    , EQUIPMENT_LIST       },

   { timer_8253   , 0,  3, 32, DISENGAGED, "8253 Timer Stability   ", "8253TtSs", TIMER_TEST         },
   { hware_int_map, 0,  4, 32, DISENGAGED, "Map Hardware Interrupts", "HhWwIiAa", HWARE_INTRPT_ADDRESSES},
   { env_test     , 8,  5, 32, DISENGAGED, "DOS Environment Space  ", "DdEeSs"  , DOS_ENVIRONMENT_SPACE },
   { test_cmd_com , 0,  6, 32, DISENGAGED, "COMMAND.COM Check      ", "TtCc"    , CMD_COM_INTEGRITY     },
   { math_80x87   ,10,  7, 32, DISENGAGED, "Math Coprocessor Check ", "MmCc"    , COPROCESSOR_CHECK        },
   { heap_test    ,11,  8, 32, DISENGAGED, "Heap Check (memory)    ", "HhCc"    , HEAP_CHECK            },

   { disk_check   , 2,  3, 62, DISENGAGED, "Floppy Drive A:"      , "FfDdAa"  , FLOPPY_DRIVE_A       },
   { disk_check   , 3,  4, 62, DISENGAGED, "Floppy Drive B:"      , "FfDdBb"  , FLOPPY_DRIVE_B       },
   { disk_check   , 1,  5, 62, DISENGAGED, "Fixed Drive  "FD      , "HhDdEe"  , HARD_DRIVE_C         },
   { disk_to_disk_check, 2,  6, 62, DISENGAGED, "Drives A: & B: " , "DdAaBb"  , DRIVES_A_B           },
   { disk_to_disk_check, 1,  7, 62, DISENGAGED, "Drives A: & "FD" " , "DdAaCc"  , DRIVES_A_C           },
   { disk_to_disk_check, 3,  8, 62, DISENGAGED, "Drives B: & "FD" " , "DdBbCc"  , DRIVES_B_C           },

   { no_func      ,18, 12,  6,    ENGAGED, "Output to Log File"     , "OoLlFf" , OUTPUT_TO_LOG_FILE | OPTION_CONTROL},

   { no_func      ,20, 12, 53,    ENGAGED, "Track Comm Port Output" , "TtCcPp" , TRACK_COMOUT       | OPTION_CONTROL},

   { no_func      ,23, 17, 33, DISENGAGED, "Configure Comm Port 1", "CcPp1"   , CONFIGURE_COM_1     | PROGRAM_CONTROL},
   { no_func      ,24, 18, 33, DISENGAGED, "Configure Comm Port 2", "CcPp2"   , CONFIGURE_COM_2     | PROGRAM_CONTROL},

   { no_func      ,25, 17,  6, DISENGAGED, "EXECUTE TESTS       " , "EeTt"     , EXECUTE_TESTS  | PROGRAM_CONTROL},
   { no_func      ,26, 18,  6, DISENGAGED, "View Test Output    " , "VvTtOo"   , VIEW_OUTPUT    | PROGRAM_CONTROL},
   { no_func      ,26, 19,  6, DISENGAGED, "View Execution Times" , "VvEeTt"   , VIEW_EXEC_TIMES| PROGRAM_CONTROL},
   { no_func      ,27, 20,  6, DISENGAGED, "Exit Program        " , "EeXxPp"   , EXIT_PROGRAM   | PROGRAM_CONTROL}
};

#define NUM_SERIAL_PORTS 3584

void main(int argc, char *argv[]);                 /* Local prototypes. */
int set_status_box(BMENU *main_menu, int index);
int menu_map_index(int row, int col);

#include "async.h"                                 /* ASYNC.C prototypes. */

void main(int argc, char *argv[])
{  BOOLEAN exit_program, some_engaged=FALSE, comm_port_chosen;
   BMENU  *main_menu;
   BORDER bord, test_bord;
   WHERE  loc, test_loc;
   time_t filetime;
   int    mode, columns, cursor_was_off, srow, scol, high, low;
   int    row, col, ch, scancode;
   int    index, i, fct_index[OUTPUT_TO_LOG_FILE], last_fct=0;
   int    serial_flush_timeout;
   char   inkey = (char) 1;
   double exec_times[OUTPUT_TO_LOG_FILE];

   /**********************************************************************/
   /* Get log file name from command line. Ensure all parms are present. */
   /* Assume full path will be specified - path will then be in argv[1]. */
   /**********************************************************************/
   if (argc-1 < 3)                             /* Require @ least 3 arg. */
   {  printf("CDP Command Line Syntax :\n\n");
      printf(" CDP logfile.dat 2 3 x\n");
      printf("     ³           ³ ³ ÀÄÄ Calibration Override\n");
      printf("     ³           ³ ÀÄÄÄÄ Number of lines for disk test\n");
      printf("     ³           ÀÄÄÄÄÄÄ Delay for video test (clock tics)\n");
      printf("     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Log filename\n");
      exit(-1);
   }
   video_time = atoi(argv[2]);               /* Video test delay. */
   numofstringlines = atoi(argv[3]);         /* Number of lines, disk test. */
   assert(video_time>0 && numofstringlines>0);

   /**********************************************************/
   /* Ensure existence of @ least one comm port.             */
   /* Set up default comm port setting of 2400, even         */
   /* parity, 7 data bits, 1 stop bit. Use default IRQ line. */
   /**********************************************************/
   #ifdef USE_ASYNC_C_INTERRUPTS
      assert(((_bios_equiplist() & NUM_SERIAL_PORTS) >> 9) >=1);
      assert(sio_setup(1, 4, 7, 2, 1)==0);
   #endif

   /******************************************************/
   /* Display BDM/Computer Diagnostic Program ID Screen. */
   /******************************************************/
   Program_ID_Screen();

   /***************************************************************/
   /* Execute every diagnostic routine for initial "calibration". */
   /* Check for null pointer assignments, also.                   */
   /***************************************************************/
   calibration_run = TRUE;
   reset_status_bytes = TRUE;
   assert(!utnulchk());

   /********************************************************************/
   /* Do not perform automatic calibration if 4++ cmd line parameters. */
   /********************************************************************/
   if (argc-1 < 4)
   {  for (i=0; i < OUTPUT_TO_LOG_FILE; i++)
      {  exec_times[i] = menu_map[i].pf(menu_map[i].iarg);
         if (utnulchk())                           /* Abort if NULL pointer. */
         {  fprintf(stderr, "\n\nNULL POINTER @ FUNCTION NUMBER %d\n", i);
            abort();
         }
      }
      reset_status_bytes = FALSE;
   }
   else
   {  for (i=0; i < OUTPUT_TO_LOG_FILE; i++)
         exec_times[i] = -1;                    /* No executions. */
      reset_status_bytes = TRUE;
   }

   calibration_run = FALSE;
/* _setvideomode(_DEFAULTMODE);*/

   /************************************************/
   /* Build main window that controls the program. */
   /************************************************/
   assert((main_menu = mncreate(23, 78,
                           utnybbyt(BLUE, WHITE | INTENSITY),
		                     REVERSE,           /* Highlight bar.     */
		                     NORMAL,            /* Protected items.   */
		                     NORMAL )) != NIL); /* Item descriptions. */
   assert (!b_wnerr);

   bord.type     = BBRD_DDDD | BBRD_TCT;
   bord.attr     = utnybbyt(BLUE, WHITE | INTENSITY);
   bord.ttattr   = utnybbyt(BLUE, WHITE | INTENSITY);
   bord.pttitle  = " Computer Diagnostic Program ";

   /***************************************************************/
   /*             Build the list of menu selections.              */
   /***************************************************************/
   for (index=0; index< (sizeof(menu_map)/sizeof(menu_map[0])); index++)
      assert ( mnitmkey(main_menu, menu_map[index].row, menu_map[index].col,
               MN_NOPROTECT, menu_map[index].text, menu_map[index].pkeys,
               MN_SELECT) != NIL);
                  
   /********************************************************/
   /* Determine the active page of current display device. */
   /********************************************************/
   loc.dev = (global_device = scmode(&mode,&columns,&act_page));
   loc.page = act_page;
   loc.corner.row = 1;
   loc.corner.col = 1;

   /************************************************************/
   /* Save former cursor position and size. Display main menu. */
   /************************************************************/
   cursor_was_off = sccurst(&srow,&scol,&high,&low);
   assert( mndsplay(main_menu, &loc, &bord) != NIL);

   /* Label each diagnostic category. */

   vidspmsg( 2,  2, BROWN | INTENSITY, BLUE, "I/O Devices & General");
   vidspmsg( 2, 28, BROWN | INTENSITY, BLUE, "System / Memory");
   vidspmsg( 2, 58, BROWN | INTENSITY, BLUE, "Disk Drive Tests");
   vidspmsg(11,  2, BROWN | INTENSITY, BLUE, "Output Options");
   vidspmsg(16,  6, BROWN | INTENSITY, BLUE, "Program Commands");
   vidspmsg(16, 33, BROWN | INTENSITY, BLUE, "Mandatory Output Device");

   /* Draw single lined boxes around the diagnostic categories. */

   scbox( 3,  2, 10, 26, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
   scbox( 3, 28, 10, 56, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
   scbox( 3, 58, 10, 78, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
   scbox(12,  2, 15, 77, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
   scbox(17,  6, 22, 27, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
   scbox(17, 32, 20, 56, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));

   /* Add 'status boxes'.  Mark as engaged/disengaged. */

   wnselect(main_menu->pwin);
   for (index=0; index< (sizeof(menu_map)/sizeof(menu_map[0])); index++)
   {  if ((menu_map[index].item_id & CMD_MASK) != PROGRAM_CONTROL)
         wnwrbuf(menu_map[index].row, menu_map[index].col-4, 0,
            "[ ]", -1, -1, NO_MOVE_CUR | CHARS_ONLY);

      if (menu_map[index].status_box == ENGAGED)
         wnwrbuf(menu_map[index].row, menu_map[index].col-3, 0, 
             "þ", BROWN | INTENSITY, -1, NO_MOVE_CUR | CHARS_ONLY);
   }
   assert (!b_wnerr);

   /**************************************/
   /* Build the test-in-progress window. */
   /**************************************/
   test_window = wncreate(20, 74, utnybbyt(BLACK, WHITE | INTENSITY));
   assert(test_window != NIL);
   assert (!b_wnerr);

   test_bord.type     = BBRD_SSSS | BBRD_TCT;
   test_bord.attr     = utnybbyt(BLACK, WHITE | INTENSITY);
   test_bord.ttattr   = utnybbyt(BLACK, WHITE | INTENSITY);
   test_bord.pttitle  = " DIAGNOSTIC(S) WINDOW ";
   test_loc.dev       = loc.dev;
   test_loc.page      = loc.page;
   test_loc.corner.row = 2;
   test_loc.corner.col = 3;

   /************************************************/
   /* Open comm port output tracking file.         */
   /* Open log file. Indicate time/date of append. */
   /************************************************/
   assert((trackcom_file = fopen("TRACKCOM.DAT", "wt")) != NULL);
   assert((log_file = fopen(argv[1], "a+t")) != NULL);
   time(&filetime);
   fprintf(log_file, "FILE : %s APPENDED ON %s", argv[1], ctime(&filetime));
    
   /********************************************************************/
   /*                 Read menu & process selections.                  */
   /********************************************************************/
   comm_port_chosen=FALSE;          /* Force selection of comm port.   */
   exit_program = FALSE;            /* Force @ least 1 loop iteration. */
   row = col = 0;                   /* Begin with 1st menu selection.  */
   status_line("Use  keys or first letter to Select Menu Item ³ ENTER to toggle ON/OFF");
   do
   {
      assert ( mnread(main_menu, row, col, &row, &col, &ch, &scancode,
               MN_UNKNOWN_BEEP | MN_KEEP_HIGHLIGHT ) == WN_NO_ERROR);

      if (scancode == KB_S_N_ENTER)
      {  int execute = TRUE;
         index = menu_map_index(row, col);

         /***************************************************************/
         /* Process the PROGRAM_CONTROL items, toggle status of others. */
         /***************************************************************/
         switch (menu_map[index].item_id & ID_MASK)
         { case CONFIGURE_COM_1:
               comm_port_menu(1);
               comm_port_chosen = TRUE;
               break;

           case CONFIGURE_COM_2:
               comm_port_menu(2);
               comm_port_chosen = TRUE;
               break;
            
           case EXECUTE_TESTS :
               if (!comm_port_chosen)
               {  status_line("You MUST configure COM1 or COM2 at this time !");
                  utspkr(540); utsleep(8); utspkr(0);
                  continue;
               }

               /************************************************/
               /* Prevent infinite loop when no tests engaged. */
               /************************************************/
               some_engaged = FALSE;
               last_fct=0;                               /* Reset last fct #. */
               for (i=0; i < OUTPUT_TO_LOG_FILE; i++)
               if (menu_map[i].status_box == ENGAGED)
               {  some_engaged = TRUE;
                  fct_index[last_fct]=i;                 /* Load fct index[]. */
                  last_fct++;
               }

               if (!some_engaged)
               {  status_line("NO TESTS WERE SELECTED ... Press Any Key To Continue");
                  utspkr(540); utsleep(11); utspkr(0);
                  kbflush();
                  while(!kbhit());
                  kbflush();
                  continue;
               }

               /*************************************/
               /* Pop up the test reporting window. */
               /*************************************/
               assert(wndsplay(test_window, &test_loc, &test_bord) != NIL);

               /******************************************/
               /* Execute each function that is engaged. */
               /* Loop until ESCAPE is detected.         */
               /******************************************/
               do
               { for (i=0; i < last_fct; i++)
                 {  status_line("Waiting for 'S' or ESC from comm port/keyboard ... ");
                    inkey = wait_for_char("Ss");
                    if (inkey == (char) 27) break;
                    menu_map[fct_index[i]].pf(menu_map[fct_index[i]].iarg);   /* Function to execute. */
                    sio_flush(serial_flush_timeout);
                 }
                 wnselect(test_window);
                 wnprintf("---------------- End of Test Loop Iteration ---------------\n");
               } while (inkey != (char) 27);

               assert(wnremove(test_window)!=NIL);

               /* Compensate for a Blaise C-Tools Plus idiosyncrasy. */

               scbox( 3, 58, 10, 78, 0, -1, utnybbyt(BLUE, BROWN | INTENSITY));
               reset_status_bytes = FALSE;
               break;

           case VIEW_OUTPUT   :
               assert(wndsplay(test_window, &test_loc, &test_bord) != NIL);
               status_line("Viewing Test Window - PRESS ANY KEY TO CONTINUE ...");
               kbflush();
               while(!kbhit());
               kbflush();
               assert(wnremove(test_window)!=NIL);
               break;

           case VIEW_EXEC_TIMES :
               assert(wndsplay(test_window, &test_loc, &test_bord) != NIL);
               status_line("Viewing approximate Diagnostics Execution Times - PRESS ANY KEY TO CONTINUE");
               kbflush();

               wnselect(test_window);
               wnprintf("Initial Diagnostics Execution Times (NOT MOST RECENT)\n");
               for (i=0; i < OUTPUT_TO_LOG_FILE; i++)
               {  wnprintf("Diagnostic # %2d %23s ", i+1, menu_map[i].text);
                  if (exec_times[i]<0.0)
                     wnprintf("ERROR:EXECUTION TIME UNAVAILABLE\n");
                  else
                     wnprintf("%4.2f seconds\n", exec_times[i]);
               }
               while(!kbhit());
               kbflush();
               assert(wnremove(test_window)!=NIL);
               break;

           case EXIT_PROGRAM :
               exit_program = TRUE;
               break;

           default:         
               status_line("ENTER toggle's engagement of the function ...");
               set_status_box(main_menu, index);
         }
      }

      assert(!utnulchk());

   } while (! exit_program);

   #ifdef USE_ASYNC_C_INTERRUPTS
      sio_stop();
   #endif

   /***************************************************/
   /* Close all files. Indicate time/date of closing. */
   /***************************************************/
   time(&filetime);
   fprintf(log_file, "FILE : %s CLOSED ON %s", argv[1], ctime(&filetime));
   fcloseall();

   /***************************************************/
   /* Remove the window. Restore the screen & cursor. */
   /***************************************************/
   assert(wndstroy(test_window)==WN_NO_ERROR);
   mndstroy(main_menu);
   sccurset(srow,scol);
   scpgcur(cursor_was_off,high,low,CUR_NO_ADJUST);

   printf("CDP Terminated Normally\n");
   printf("Test Descriptive Log File: %s\n", argv[1]);
   printf("Comm Port Output Log File: TRACKCOM.DAT\n");

   assert(!utnulchk());
}

/***************************************************************/
/* AUTHOR   : James C. Deen, Jr.                               */
/* PURPOSE  : Returns index for which menu_map[index].row &    */
/*       menu_map[index].col match the parameters row & col.   */
/* ALGORITHM:                                                  */
/*       Searches menu_map[] global array.                     */
/* INPUT    :                                                  */
/*       row - to be matched with menu_map[].row .             */
/*       col - to be matched with menu_map[].col .             */
/* NOTES    :                                                  */
/*       index == -1 indicates unsuccessful search.            */
/***************************************************************/
int menu_map_index(int row, int col)
{  int i, index=-1;
   for (i=0; i< (sizeof(menu_map)/sizeof(menu_map[0])); i++)
      if ((row == menu_map[i].row) && (col == menu_map[i].col)) 
      {  index = i;
         break;
      }
   assert(index != -1);       /* Logic error if so. */
   return(index);
}

/*******************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                   */
/* PURPOSE  : Set menu_map[index].status_box & changes the "status */
/*       box" display appropriately.                               */
/* ALGORITHM:                                                      */
/*       Toggle menu_map[index].status_box .                       */
/*       Write "X" in YELLOW @ (menu_map[].row,menu_map[].col-3).  */
/* INPUT    : index - menu_map[] index.                            */
/* RETURNS  : b_wnerr .                                            */
/*******************************************************************/
int set_status_box(BMENU *main_menu, int index)
{  
   wnselect(main_menu->pwin);

   if (menu_map[index].status_box == DISENGAGED)
   {  menu_map[index].status_box = ENGAGED;
      wnwrbuf(menu_map[index].row, menu_map[index].col-3, 0, "þ",
         BROWN | INTENSITY, -1, NO_MOVE_CUR | CHARS_ONLY);
   }
   else
   {  menu_map[index].status_box = DISENGAGED;
      wnwrbuf(menu_map[index].row, menu_map[index].col-3, 0, " ",
         -1, -1, NO_MOVE_CUR | CHARS_ONLY);
   }
   return(b_wnerr);
}

/**********************************************************************/
/* TEMPORARY - indicates menu item not coupled to a working function. */
/**********************************************************************/
double not_avail(int fno)
{
   if (calibration_run) return(-1);
   tlprintf("Diagnostic No %d Not Available (Yet ...)", fno+1);
   reportf("SX%.2d_NOT_E", fno+1);
   return(0);
}

/******************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                  */
/* PURPOSE  : Provide trap to identify functions unintentionally  */
/*       called by referencing menu_map[].pf(i) .                 */
/* ALGORITHM: Use an assert() which will ALWAYS be FALSE.         */
/******************************************************************/
double no_func(int fno)
{  tlprintf("FUNCTION # %d CALLED ERRONEOUSLY.\n", fno);
   reportf("SBADFUNC%dE", fno);
   fprintf(stderr, "FUNCTION # %d CALLED ERRONEOUSLY.\n", fno+1);
   assert(1==0);
   return(0.0);
};

