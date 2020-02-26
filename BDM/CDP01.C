/************************************************************************/
/*                              CDP01.C                                 */
/************************************************************************/

/************************************************************************/
/* PURPOSE:    "I/O Devices & General" test category functions.         */ 
/* AUTHOR(S):  James C. Deen, Jr.                                       */
/* LOCATION:   BDM, Huntsville, Alabama                                 */
/* MAINTENANCE:                                                         */
/*    Created on Oct. 1, 1991                                           */
/************************************************************************/

#include "cdp.h"

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  :                                                        */
/* ALGORITHM: Bits 4 & 5 are masked out.                             */
/* INPUT    :                                                        */
/* OUTPUT   :                                                        */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/*********************************************************************/
double equip_list(int dummy)              /* Function No: 06 */
{  static unsigned old_eq=0;
   unsigned eq;
   long t0, t1;
   char buffer[3][50];

   /***************************************************************/
   /* Retrieve the equipment list via BIOS call. Time the process */
   /***************************************************************/
   _bios_timeofday(_TIME_GETCLOCK, &t0);
   eq = _bios_equiplist() | 0x30;
   _bios_timeofday(_TIME_GETCLOCK, &t1);

   if (reset_status_bytes) old_eq = eq;

   reportf("S006%4.2fE", difftimer(t1,t0));
   tlprintf("Test 06 Bios Equipment List Execution Time %4.2f sec", difftimer(t1,t0));

   if (old_eq != eq)
   {  reportf(buffer[1], "S20600%.4XE", old_eq);
      reportf(buffer[2], "S20600%.4XE", eq    );
      tlprintf("Old Status Byte %.4XE", old_eq);
      tlprintf("New Status Byte %.4XE", eq    );
   }

   return(difftimer(t1,t0));           /* For now - make more meaningful later. */
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  :                                                        */
/* ALGORITHM:                                                        */
/* INPUT    :                                                        */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/********************************* TEST # 01 *************************/
double read_kb_flags(int dummy)
{  static unsigned old_kb=0;
   unsigned kb;
   long t0, t1;

   _bios_timeofday(_TIME_GETCLOCK, &t0);
   kb = _bios_keybrd(_KEYBRD_SHIFTSTATUS);
   _bios_timeofday(_TIME_GETCLOCK, &t1);

   if (reset_status_bytes) old_kb = kb;

   reportf ("S002%4.2fE", difftimer(t1,t0));
   tlprintf("Test 02 Keyboard Flags Access Time %4.2f sec", difftimer(t1,t0));

   if (old_kb != kb)
   {  reportf("S20200%.4XE", old_kb);
      reportf("S20200%.4XE", kb    );
      tlprintf("Test 02 OLD Keyboard Flags Byte : %.4X", old_kb);
      tlprintf("Test 02 NEW Keyboard Flags Byte : %.4X", kb    );
   }
   else
      tlprintf("Test 02 NEW Keyboard Flags Byte : %.4X", kb    );
   
   return(difftimer(t1,t0));
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  : Report status bits of the various com port registers.  */
/* ALGORITHM: inp() calls referenced to base address stored in BIOS. */
/* INPUT    : comport - 1: com port 1;  2:com port 2; etc.           */
/* OUTPUT   : one or more of the following output functions :        */
/*                report(), reportf(), and/or tlprintf().            */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/*********************************************************************/
#include "async.h"

double cport_status(int port_no)
{  COMM_REGS comm_regs;
   static COMM_REGS old_comm_regs[2];
   int ret_code;
   long t0, t1;

   assert(port_no >=1 && port_no<=2);  /* Limit code to 2 ports, for now. */

   _bios_timeofday(_TIME_GETCLOCK, &t0);
   ret_code = read_comm_regs(port_no, &comm_regs);
   _bios_timeofday(_TIME_GETCLOCK, &t1);
   
   if (reset_status_bytes) old_comm_regs[port_no-1] = comm_regs;

   /*******************************************/
   /* Indicate if comm port is not installed. */
   /*******************************************/
   if (ret_code != 0)
   {  reportf("S1%.2d0001E", port_no + 2);       /* S1030001E or S1040001E */
      tlprintf("ERROR: COMM PORT %d NOT installed.", port_no);
      return(-1.0);                             /* UNSuccessful return. */
   }
   
   reportf("S0%.2d%4.2fE", port_no+2, difftimer(t1,t0));
   tlprintf("Test %2d Comm Port %d Registers Access Time : %4.2f sec", port_no+2, port_no, difftimer(t1,t0));

   if (calibration_run == FALSE)
   {
      /* Send status bytes changes to MacIntosh. */

      if (old_comm_regs[port_no-1].int_enable_reg   != comm_regs.int_enable_reg  ) reportf("S2%.2d01%.4XE", port_no+2, comm_regs.int_enable_reg );
      if (old_comm_regs[port_no-1].int_id_reg       != comm_regs.int_id_reg      ) reportf("S2%.2d02%.4XE", port_no+2, comm_regs.int_id_reg      );
      if (old_comm_regs[port_no-1].line_ctrl_reg    != comm_regs.line_ctrl_reg   ) reportf("S2%.2d03%.4XE", port_no+2, comm_regs.line_ctrl_reg   );
      if (old_comm_regs[port_no-1].modem_ctrl_reg   != comm_regs.modem_ctrl_reg  ) reportf("S2%.2d04%.4XE", port_no+2, comm_regs.modem_ctrl_reg  );
      if (old_comm_regs[port_no-1].line_status_reg  != comm_regs.line_status_reg ) reportf("S2%.2d05%.4XE", port_no+2, comm_regs.line_status_reg );
      if (old_comm_regs[port_no-1].modem_status_reg != comm_regs.modem_status_reg) reportf("S2%.2d06%.4XE", port_no+2, comm_regs.modem_status_reg);

      /* Send status bytes changes to screen and log file. */

      if (old_comm_regs[port_no-1].int_enable_reg   != comm_regs.int_enable_reg  ) tlprintf("Test %.2d Interrupt Enable Register : %.4X", port_no+2, comm_regs.int_enable_reg );
      if (old_comm_regs[port_no-1].int_id_reg       != comm_regs.int_id_reg      ) tlprintf("Test %.2d Interrupt ID Register     : %.4X", port_no+2, comm_regs.int_id_reg      );
      if (old_comm_regs[port_no-1].line_ctrl_reg    != comm_regs.line_ctrl_reg   ) tlprintf("Test %.2d Line Control Register     : %.4X", port_no+2, comm_regs.line_ctrl_reg   );
      if (old_comm_regs[port_no-1].modem_ctrl_reg   != comm_regs.modem_ctrl_reg  ) tlprintf("Test %.2d Modem Control Register    : %.4X", port_no+2, comm_regs.modem_ctrl_reg  );
      if (old_comm_regs[port_no-1].line_status_reg  != comm_regs.line_status_reg ) tlprintf("Test %.2d Line Status Register      : %.4X", port_no+2, comm_regs.line_status_reg );
      if (old_comm_regs[port_no-1].modem_status_reg != comm_regs.modem_status_reg) tlprintf("Test %.2d Modem Status Register     : %.4X", port_no+2, comm_regs.modem_status_reg);
   }
   
   return(difftimer(t1,t0));     /* Successful return. */
}     
      
/*********************************************************************/
/* PURPOSE  : Test COMMAND.COM .                                     */
/* ALGORITHM: Execute DOS cmd "ver > nul" via system call.           */
/* INPUT    : dummy - not used.                                      */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/** TEST # 09 ********************************************************/
double test_cmd_com(int dummy)
{  int cmd_com_result, old_errno;
   long t1, t0;
   double exec_time;
   old_errno = errno;                        /* Save pre-existing errno. */

   _bios_timeofday(_TIME_GETCLOCK, &t0);
   cmd_com_result = system("ver > nul");
   _bios_timeofday(_TIME_GETCLOCK, &t1);
   exec_time = difftimer(t1,t0);

   if (cmd_com_result == 0)
   {  tlprintf("Test 10 COMMAND.COM ver > nul Execution Time %4.2f sec", exec_time);
      reportf("S010%4.2fE", exec_time);
   }
   else
   {  switch (errno)
      { case E2BIG:
            report("S1100001E");
            tlprintf("Test 10 COMMAND.COM ERROR: Argument List > 128 Bytes");
            break;
        case ENOENT:
            report("S1100002E");
            tlprintf("Test 10 COMMAND.COM ERROR: Not Found");
            break;
        case ENOEXEC:
            report("S1100003E");
            tlprintf("Test 10 COMMAND.COM ERROR: Invalid Format - Not Executable");
            break;
        case ENOMEM:
            report("S1100004E");
            tlprintf("Test 10 COMMAND.COM ERROR: Insufficient or Corrupted Memory");
            break;
        default:
            report("S1100005E");
            tlprintf("Test 10 COMMAND.COM ERROR: Unexpected Error");
      }
   }
   
   if (old_errno != errno) errno = 0;        /* Reset errno. */

   return(exec_time);
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  : Record hardware interrupt vectors for later comparison.*/
/* ALGORITHM: Reads vectors @ hex addresses 0,2,4,8-F, & 70-77.      */
/* INPUT    : internal.                                              */
/* OUTPUT   : CDP I/O devices.                                       */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/*********************************************************************/
double hware_int_map(int dummy)
{  static hw_intrpt_list[] =
   { 0x0, 0x2, 0x4, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
     0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77
   };
   static void far *old_interrupt_map[sizeof(hw_intrpt_list)/sizeof(hw_intrpt_list[0])];
   void far *new_interrupt_map[sizeof(hw_intrpt_list)/sizeof(hw_intrpt_list[0])];
   int i;
   long t1, t0;

   if (reset_status_bytes)
   {  for (i=0; i < sizeof(hw_intrpt_list)/sizeof(hw_intrpt_list[0]); i++)
         old_interrupt_map[i] = _dos_getvect(hw_intrpt_list[i]);
   }

   _bios_timeofday(_TIME_GETCLOCK, &t0);

   for (i=0; i < sizeof(hw_intrpt_list)/sizeof(hw_intrpt_list[0]); i++)
      new_interrupt_map[i] = _dos_getvect(hw_intrpt_list[i]);
   _bios_timeofday(_TIME_GETCLOCK, &t1);

   tlprintf("Test 08 Hardware Interrupts Access Time : %4.2f sec", difftimer(t1, t0));
   reportf("S008%4.2fE", difftimer(t1, t0));

   for (i=0; i < sizeof(hw_intrpt_list)/sizeof(hw_intrpt_list[0]); i++)
   {
      if (new_interrupt_map[i] != old_interrupt_map[i])
      {  tlprintf("Test 08 Hardware Interrupt Vector %x changed from %p to %p",
            hw_intrpt_list[i], old_interrupt_map[i], new_interrupt_map[i]);

         reportf("S208%2d%.4XE", hw_intrpt_list[i], new_interrupt_map[i]);

         old_interrupt_map[i] = new_interrupt_map[i];
      }
   }
   return(difftimer(t1,t0));
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  : Test stability of 8253 channel 2 stability.            */
/* ALGORITHM: 1) Halt any 8253 channel 2 activity.                   */
/*            2) 8253 channel 2 is set to an arbitrary frequency.    */
/*            3) Loop to let any residual decrementing complete.     */
/*            4) Read lo/hi bytes 100 times. If they're all the      */
/*               same, then timer is considered stable - ie: since   */
/*               channel 2 was manually disabled, no more activity   */
/*               should occur.                                       */
/* INPUT    : internal.                                              */
/* OUTPUT   : CDP I/O devices.                                       */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/*************************** Test No: 07 *****************************/

#define PB8255	               0x61        /* Port B of the 8255 chip.   */
#define TIMER2_REGISTER_8253  0x42
#define CONTROL_REGISTER_8253 0x43
#define CLOCK_FREQ            1193180L

double timer_8253(int dummy)
{  int i;
   unsigned int hi0, hi1, lo0, lo1;
   long t0, t1;
   BOOLEAN err_found = FALSE;

   _disable();

	outp(PB8255, utinp (PB8255) & ~3);     /* Halt chan 2 timer operations. */

   outp(CONTROL_REGISTER_8253, 0xB6);
   outp(TIMER2_REGISTER_8253, utlobyte((unsigned int)(CLOCK_FREQ/6553)));
   outp(TIMER2_REGISTER_8253, uthibyte((unsigned int)(CLOCK_FREQ/6553)));

   /********************************************/
   /* Wait for timer to stabilize since 1 or 2 */
   /* decrements may occur after loading.      */
   /********************************************/
   _bios_timeofday(_TIME_GETCLOCK, &t0);

   /**************************************/
   /* Wait for timer count to stabilize. */
   /**************************************/
   for (i=0; i<10; i++)                   
   {  lo0 = inp(TIMER2_REGISTER_8253);
      hi0 = inp(TIMER2_REGISTER_8253);
   }

   /*********************************************/
   /* Loop 100 times. If all readings match up, */
   /* then timer is considered "stable" .       */
   /*********************************************/
   for (i=0; i<100; i++)
   {  lo1 = inp(TIMER2_REGISTER_8253);
      hi1 = inp(TIMER2_REGISTER_8253);
      if (lo0 != lo1)                     /* Low byte was unstable. */
      {  report("S1070001E");
         err_found = TRUE;
      }
      if (hi0 != hi1)                     /* High byte was unstable. */
      {  report("S1070002E");
         err_found = TRUE;
      }

      if (err_found) break;               /* break loop for any errors. */
   }

   _bios_timeofday(_TIME_GETCLOCK, &t1);
   if (!err_found)
   {  
      reportf("S007%4.2fE", difftimer(t1,t0));
      tlprintf("Test 07 8253 Timer Stability Execution Time %4.2f", difftimer(t1,t0));
      _enable();
      return(difftimer(t1,t0));
   }
   else
   {  _enable();
      return(-1.0);
   }
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  : Test stability of 80x87 math coprocessor.              */
/* ALGORITHM: 1) Clear any previous error status bits.               */
/*            2) Read the specific status bits up to 100 times.      */
/*               If any are non-zero, then send the appropriate      */
/*               error code.                                         */
/* INPUT    : internal.                                              */
/* OUTPUT   : CDP I/O devices.                                       */
/* RETURNS  : Execution time, or -1.0 for error.                     */
/*********************************************************************/
#include <float.h>
double math_80x87 (int dummy)
{  long t0, t1;
   unsigned fpstatus;
   int i, eflag=0;

   if (!((_bios_equiplist()>>1)&1))
   {  report("S1110001E");             /* Math Coprocessor absent. */
      eflag = 1;
   }
   else
   {  float a;                         /* Used to test loss of precession. */
      double b=1.e-40;

      _bios_timeofday(_TIME_GETCLOCK, &t0);
      fpstatus = _clear87();

      /************************************************************************/
      /* Check for occurence of all errors. Since reportf() returns number of */
      /* chars written, then an eflag >=1 singals @ least 1 error.            */
      /************************************************************************/
      for (i=0; i<100; i++)
      {  fpstatus = _status87();
         if ((fpstatus & SW_INVALID       ) == SW_INVALID       ) eflag += reportf("S1110002E");
         if ((fpstatus & SW_DENORMAL      ) == SW_DENORMAL      ) eflag += reportf("S1110003E");
         if ((fpstatus & SW_ZERODIVIDE    ) == SW_ZERODIVIDE    ) eflag += reportf("S1110004E");
         if ((fpstatus & SW_OVERFLOW      ) == SW_OVERFLOW      ) eflag += reportf("S1110005E");
         if ((fpstatus & SW_UNDERFLOW     ) == SW_UNDERFLOW     ) eflag += reportf("S1110006E");
         if ((fpstatus & SW_INEXACT       ) == SW_INEXACT       ) eflag += reportf("S1110007E");
         if ((fpstatus & SW_UNEMULATED    ) == SW_UNEMULATED    ) eflag += reportf("S1110008E");
         if ((fpstatus & SW_SQRTNEG       ) == SW_SQRTNEG       ) eflag += reportf("S1110009E");
         if ((fpstatus & SW_STACKOVERFLOW ) == SW_STACKOVERFLOW ) eflag += reportf("S1110010E");
         if ((fpstatus & SW_STACKUNDERFLOW) == SW_STACKUNDERFLOW) eflag += reportf("S1110011E");
      }

      /*******************************************/
      /* Generate a loss of precision condition. */
      /* Check UNDERFLOW status bits.            */
      /*******************************************/
      _clear87();
      a=b;
      if((_status87() & 0x18) == 0x0)
         eflag += reportf("S1110012E");

      _bios_timeofday(_TIME_GETCLOCK, &t1);
      reportf("S011%4.2fE", difftimer(t1,t0));
   }

   if (eflag)
   {  tlprintf("Test 11 Math Coprocessor Error(s)");
      return(-1.0);
   }
   else
   {  tlprintf("Test 11 Math Coprocessor Check Execution Time: %5.2f seconds", difftimer(t1,t0));
      return(difftimer(t1,t0));
   }
}
