/********************************************************************/
/* FILE:        ASYNC.H (formerly SERIO.H)                          */
/*                                                                  */
/* DESCRIPTION: Header file for programs that use ASYNC.C (SERIO.C) */
/*    Reorganized October 9, 1991 by James C. Deen, Jr.             */
/********************************************************************/

#ifndef SERIOHEADER
   #define SERIOHEADER 

   typedef struct
   {  int data_reg        ;
      int int_enable_reg  ;
      int int_id_reg      ;
      int line_ctrl_reg   ;
      int modem_ctrl_reg  ;
      int line_status_reg ;
      int modem_status_reg;
   } COMM_REGS;
   
   int sio_put(int c);        /* Write a character. returns 0 if successful.             */ 
   int sio_charready(void);   /* Check for character available (1 means char ready).     */ 
   int sio_get(void);         /* Wait for character and return it. returns -1 for break. */ 
   void sio_flush(int timeout_sec);    /* Flush serial port. */ 

   #define GOOD_SIO_SETUP 0
   #define BAD_BAUD       1
   #define BAD_BITS       2
   #define BAD_STOPS      3
   #define BAD_PARITY     4

   /*************************************************/
   /* Comm port register base address port offsets. */
   /*************************************************/

   #define DATA_REG         0
   #define INT_ENABLE_REG   1          /* Interrupt Enable Register */
   #define INT_ID_REG       2          /* Interrupt Id  Register    */
   #define LINE_CTRL_REG    3          /* Line  Control Register    */
   #define MODEM_CTRL_REG   4          /* Modem Control Register    */
   #define LINE_STATUS_REG  5          /* Line  Status  Register    */
   #define MODEM_STATUS_REG 6          /* Modem Status  Register    */

   /*******************************************************************/
   /*                   ASYNC.C Function prototypes                   */
   /*******************************************************************/
   int sio_setup(int comport,int baud,int bits,int parity,int stops);
   void sio_stop(void);
   void sio_sendbreak(void);
   int read_comm_regs(int comport, COMM_REGS *comm_regs);     /* By DEEN */

   /*********************************************************************/
   /*                        Stuff from COMPAT.H                        */
   /*********************************************************************/
   #define FARNULL (void far *)0
   #define __FAR far
   
   #define NEARNULL (void near *)0
   
   #define INTREGS unsigned Res, unsigned Rds, unsigned Rdi, unsigned \
     Rsi, unsigned Rbp, unsigned Rsp, unsigned Rbx, unsigned Rdx,\
     unsigned Rcx, unsigned Rax, unsigned Rip, unsigned Rcs, unsigned Rfl

   #define getvect(x) _dos_getvect(x)
   #define setvect(x,y) _dos_setvect(x,y)
   #define bioskey(a) _bios_keybrd(a)
   #define MK_FP(s,o) ((void far *)((((unsigned long)(s))<<16) | \
           ((unsigned long) (o))))
   
   /*********************************************
    * Microsoft must link with int86fx.asm,     *
    * if int86fx or int86f is used ...          *
    * #define int86f(a,b,c) int86fx(a,b,c,NULL) *
    *********************************************/

#endif

/* End of header */ 
