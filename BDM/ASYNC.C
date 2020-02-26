/*****************************************************************/
/* FILE:          ASYNC.C                                        */
/* PURPOSE:       A library for interrupt driven serial I/O.     */
/* SOURCE:        DOS 5: A Developer's Guide - file: SERIO.C     */
/* MAINTENANCE:                                                  */
/*    October 9 ,1991:  James C. Deen, Jr.                       */
/*       Reorganized source code format/indentation from SERIO.C */
/*       Deleted explicit references to POWERC & TURBOC.         */
/*                                                               */
/*    October 22, 1991: James C. Deen, Jr.                       */
/*       sio_setup() - Deleted capability to set IRQ explicitly. */
/*       Removed irq parameter from function call.               */
/*****************************************************************/

#undef NDEBUG           /* Enable the assert macro. */

#include <assert.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>     /* Needed for abort() definintion. */
#include <time.h>

#include "async.h"

static void xmit(void);
static void rcv(void);
static void msr(void);
static void linestat(void);

#define BUFFERLEN 1024

#define FILL 32            /* If <FILL bytes are free in rcvbuf, send XOFF. */
#define BRKTIME 5          /* Ticks to hold a break.        */
static int basead;         /* Base address of UART I/O.     */
static int irqmask,irqnum; /* IRQ & mask bit for PIC.       */

#define XOFF 19
#define XON 17

/***********************************************************************/
/*          Global variables of interest to user's program.            */
/***********************************************************************/

int sio_error=0, sio_errct=0;       /* Error words. */

int sio_break=0;        /* Set to 1 when break occurs.                 */
int sio_linestat;       /* Line status.                                */
int sio_modemstat;      /* Modem status.                               */
int sio_doxoff=1;       /* Set xon/xoff mode on.                       */
int sio_brkmode=0;      /* If 1, sio_get() returns -1 if break occured */

/*************************************************************************/
/* NOTE:                                                                 */
/*    Base register offsets (such as MODEM_CTRL_REG) defined in ASYNC.H. */
/*************************************************************************/

#define circinc(a,b) circadd(a,b,1)

#pragma check_stack(off)

static void (interrupt far *oldirq)();
/**************************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                          */
/* PURPOSE  : Interrupt Service Routine (ISR) for UART interrupt.         */
/* ALGORITHM: Call xmit(), rcv(), linestat(), or msr() depending upon     */
/*            contents of Interrupt ID Register.                          */
/* INPUT    : None. Invoked upon async interrupt.                         */
/* OUTPUT   : PIC (Programmable Interrupt Controller) .                   */
/* RETURNS  : Nothing.                                                    */
/**************************************************************************/
void far interrupt comint(void);
void far interrupt comint(void)
{
   int intstat = inp(basead+INT_ID_REG);  /* Read Interrupt ID register. */

   if (intstat&1)
      oldirq();                     /* No interrupt pending. */
   else
   {  while (!(intstat&1))
      {
         if (intstat==2) xmit();
         else if (intstat==4) rcv();
         else if (intstat==6) linestat();
         else msr();

         intstat=inp(basead+INT_ID_REG);
      }
   }
   outp(0x20,0x20);                 /* Reset PIC. */
}


/* Handle Modem Status interrupts. */
static void msr()
{
   sio_modemstat=inp(basead+MODEM_STATUS_REG);
}

/* Handle Line Status interrupts. */
static void linestat()
{
   sio_linestat = inp(basead+LINE_STATUS_REG);
   if (sio_linestat&0x10) sio_break=1;
}

/*********************************************/
/*       Define two circular buffers         */
/*********************************************/
static char     xmitbuf[BUFFERLEN];
static unsigned xmithead=0, xmittail=0;
static char     rcvbuf[BUFFERLEN];
static unsigned rcvhead=0, rcvtail=0;
static int      sentxoff=0, sendxoff=0, sendxon=0;
static int      gotxoff=0;

/*********************************************************************/
/* PURPOSE: Increment buffer pointer circularly.                     */
/* ALGORITHM:  1) ptr = ptr + offset                                 */
/*             2) 0 <= ptr < max                                     */
/* NOTES:   offset == 1 when called via circinc() macro.             */
/*********************************************************************/
static unsigned circadd(unsigned ptr, unsigned max, unsigned offset)
{
   ptr+=offset;
   while (ptr>=max) ptr-=max;    
   return ptr;
}

/*********************************************************************/
/* PURPOSE: Attempt to transmit a character.                         */
/*********************************************************************/
static void xmit(void)
{
   int c;
   if (!(inp(basead+LINE_STATUS_REG)&0x20)) return;      /* False alarm. */

   if (sendxoff)
   {
      sendxoff=0;
      outp(basead+DATA_REG,XOFF);
      sentxoff=1;
      return;
   }

   if (sendxon)
   {
      sendxon=0;
      outp(basead+DATA_REG,XON);
      sentxoff=0;
      return;
   }

   if ((sio_doxoff && gotxoff) || xmithead==xmittail) return;

   c = xmitbuf[xmittail];
   xmittail=circinc(xmittail, sizeof(xmitbuf));
   outp(basead+DATA_REG, c);
}

/***********************************/
/* Attempt to receive a character. */
/***********************************/
static void rcv(void)
{
   int tmp;

   if (!(inp(basead+LINE_STATUS_REG)&1)) return;      /* False alarm. */

   tmp=circinc(rcvhead,sizeof(rcvbuf));

   if ((rcvhead>rcvtail&&tmp>rcvtail&&tmp<rcvhead) ||
       (rcvhead<rcvtail&&tmp>rcvtail))
   {
      /* buffer overflow */
      sio_error=1;
      sio_errct++;
      return;
   }

   tmp=circadd(rcvtail,sizeof(rcvbuf),FILL);
   if ((rcvhead>rcvtail&&tmp>rcvtail&&tmp<rcvhead) ||
       (rcvhead<rcvtail&&tmp>rcvtail))
   {
      if (sio_doxoff)
         {
            sendxoff=1;
            /* wait for tranmit buffer to empty */
            while (!(inp(basead+LINE_STATUS_REG)&0x20));
            xmit();
         }
   }
   else
   {
      if (sentxoff)
      {
         sendxon=1;
         /* wait for tranmit buffer to empty */
         while (!(inp(basead+LINE_STATUS_REG)&0x20));
         xmit();
      }
   }

   rcvbuf[rcvhead] = inp(basead+DATA_REG);
   if (sio_doxoff)
   {
      if (rcvbuf[rcvhead]==XOFF) gotxoff=1;
      if (rcvbuf[rcvhead]==XON) gotxoff=0;
   }
   rcvhead=circinc(rcvhead,sizeof(rcvbuf));
}

#pragma check_stack()            /* Reset stack checking to default. */

/*************************/
/*      Time delay       */
/*************************/
static void t_delay(int ticks)
{
   clock_t tick0;
   tick0=clock()+ticks;
   while (clock()<tick0);
}

/***************************************************************************/
/*                       User interface routines                           */
/***************************************************************************/

/***********************************************/
/* Write a character. Returns 0 if successful. */
/***********************************************/
int sio_put(int c)
{
   int tmp;
   tmp = circinc(xmithead, sizeof(xmitbuf));

   /******************************************************/
   /*             Check for buffer overflow              */
   /******************************************************/
   if ((xmithead>xmittail && tmp>xmittail && tmp<xmithead) ||
       (xmithead<xmittail && tmp>xmittail)                 ) return 1;
   xmitbuf[xmithead] = c;
   xmithead = tmp;

   /************************************************************/
   /* Call xmit in case transmitter has been idle for a while. */
   /* If transmitter is busy, xmit won't do anything.          */
   /************************************************************/
   xmit();
   return 0;
}


/******************************************************/
/* Check for character available (1 means char ready) */
/******************************************************/
int sio_charready()
{
   return rcvhead!=rcvtail;
}

/**********************************************************/
/* Wait for character and return it. returns -1 for break */
/**********************************************************/
int sio_get()
{
   int c;
   while (rcvhead==rcvtail) if (sio_brkmode&&sio_break)
   {
      sio_break=0;
      return -1;
   }
   c=rcvbuf[rcvtail];
   rcvtail=circinc(rcvtail,sizeof(rcvbuf));
   return c;
}

/******************************/
/*       Baud Rate table      */
/******************************/
static unsigned int brtable[] = { 0x417,0x300,0x180,0x60,0x30,0x18,0xc,0x6 };

/**************************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                          */
/* PURPOSE  : Read the various com port registers.                        */
/* ALGORITHM: inp() calls referenced to base address stored in BIOS.      */
/* INPUT    :                                                             */
/*    comport   - 1, 2, 3, or 4. Corresponds to appropriate port.         */
/*    baud      - Index for baud rate table. 0 yields 110; 7 yields 19200.*/
/*    data_bits - 5, 6, 7, or 8.                                          */
/*    parity    - 0 yields NONE; 1 yields ODD; 2 yields EVEN .            */
/*    stop_bits - 0 yields 1 stop bit; 1 yields 2 stop bits .             */
/* OUTPUT   : Direct to the various async control registers.              */
/* RETURNS  : 0 - if successful.                                          */
/**************************************************************************/
int sio_setup(int comport,int baud,int data_bits,int parity,int stop_bits)
{  unsigned far *peekad;
   unsigned cntlword;
   static unsigned parity_mask[] = { 0x00,     /* No   parity. */
                                     0x08,     /* Odd  parity. */
                                     0x18 };   /* Even parity. */

   int irq = 0;               /* Use default Interrupt Request Line. */
   
   peekad=MK_FP(0x40,0);      /* Point to table of COM addresses in BIOS. */

   assert(comport>=1 && comport<=4);   /* Trap coding errors. */
   basead=peekad[--comport];           /* Get base address. */
   if (!basead) return(1);             /* Not installed. */

   /*******************************************/
   /* Set up the default IRQ:                 */
   /* 4 for COM1 or COM3. 3 for COM2 or COM4. */
   /*******************************************/
   irq = (comport&1) == 1 ? 3 : 4;

   /**************************************/
   /* Abort program if asserts not true. */
   /**************************************/
   assert ( baud>=0 && baud<=sizeof(brtable)/sizeof(int) );
   assert ( data_bits>=5 && data_bits<=8 );
   assert ( stop_bits>=1 && stop_bits<=2 );
   assert ( parity>=0 && parity<=2 );

   irqnum=irq;

   irqmask=1<<irq;                  /* Construct IRQ mask bit for PIC. */
   oldirq=getvect(irq+8);           /* Get old IRQ vector.       */
   _disable();                      /* Disable all interrupts.   */
   setvect(irq+8,comint);           /* Set new interrupt vector. */
   _enable();                       /* Enable all interrupts.    */

   /**********************************************/
   /* Force bit 7 of Line Control Register high, */
   /* which sets UART up to receive a baud rate. */
   /**********************************************/
   outp(basead+LINE_CTRL_REG,0x83); /* none/8/1 - DLAB (Data Latch Access Bit) set */

   /*************************************************************************/
   /* Set baud rate :                                                       */
   /* Feed high byte of Baud Rate Divisor into Interrupt Enable Register.   */
   /* Feed low byte of Baud Rate Divisor into Data (xmit/receive) Register. */
   /*************************************************************************/
   outp(basead+DATA_REG,brtable[baud]&0xFF);
   outp(basead+INT_ENABLE_REG,brtable[baud]>>8);

   /*********************************************/
   /* Construct control word for LINE_CTRL_REG. */
   /*********************************************/
   cntlword = parity_mask[parity];              /* Encode parity.    */
   cntlword |= 2 * (stop_bits-1);               /* Encode stop bits. */
   cntlword |= data_bits - 5;                   /* Encode data bits. */

   outp(basead+LINE_CTRL_REG , cntlword);
   outp(basead+MODEM_CTRL_REG, 0xF);            /* Enable interrupts. */
   outp(basead+INT_ENABLE_REG, 0xF);
   outp(0x21, inp(0x21)&~irqmask);

   return (GOOD_SIO_SETUP);
}


/* CALL THIS FUNCTION BEFORE PROGRAM EXIT! */
void sio_stop()
{
   _disable();
   outp(basead+INT_ENABLE_REG,0);             /* clear UART interrupts */
   outp(basead+MODEM_CTRL_REG,0);
   outp(0x21,inp(0x21)|irqmask);   /* clear PIC */
   setvect(irqnum+8,oldirq);       /* restore interrupt vector */
   _enable();
}

/* Send a break */
void sio_sendbreak()
{
   outp(basead+LINE_CTRL_REG,inp(basead+LINE_CTRL_REG)|0x40);
   t_delay(BRKTIME);
   outp(basead+LINE_CTRL_REG,inp(basead+LINE_CTRL_REG)&~0x40);
}

/*********************************************************************/
/* AUTHOR   : James C. Deen, Jr.                                     */
/* PURPOSE  : Read the various com port registers.                   */
/* ALGORITHM: inp() calls referenced to base address stored in BIOS. */
/* INPUT    : comport - 1: com port 1;  2:com port 2; etc.           */
/*            comm_regs_array - pointer to array to fill with        */
/*                register information.                              */
/* OUTPUT   : comm_regs_array - ditto.                               */
/*********************************************************************/
int read_comm_regs(int comport, COMM_REGS *comm_regs)
{  unsigned far *peekad;
   int base_address;                      /* Base address of UART I/O. */

   assert (comport>=1 && comport<=4);

   /***********************************************/
   /* Point to table of COM addresses in BIOS.    */
   /* Read the base address of com port register. */
   /***********************************************/
   peekad=MK_FP(0x40,0);      
   base_address=peekad[--comport];
   if (!base_address)
      return (-1);                     /* Com port not installed. */

   comm_regs->data_reg         = inp(base_address + DATA_REG        );
   comm_regs->int_enable_reg   = inp(base_address + INT_ENABLE_REG  );
   comm_regs->int_id_reg       = inp(base_address + INT_ID_REG      );
   comm_regs->line_ctrl_reg    = inp(base_address + LINE_CTRL_REG   );
   comm_regs->modem_ctrl_reg   = inp(base_address + MODEM_CTRL_REG  );
   comm_regs->line_status_reg  = inp(base_address + LINE_STATUS_REG );
   comm_regs->modem_status_reg = inp(base_address + MODEM_STATUS_REG);

   return(0);     /* Successful. */
}

void sio_flush(int timeout_sec)
{  time_t t1, t0;
   time(&t0);
   while(xmithead != xmittail)
   {  xmit();
      time(&t1);
      if (difftime(t1,t0) > (double) timeout_sec) break;
   }
}

