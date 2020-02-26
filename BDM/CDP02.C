#include "cdp.h"

int common_free(char *str1,char *str2);            /* Local declaration. */

/****************************************************************************/
/*                          DISK_CHECK                                      */
/* WRITTEN BY LISA PEREIRA                                                  */
/*                                                                          */
/* THIS FUNCTION TESTS EITHER THE A, B, OR C DRIVE. IN ORDER TO DISTINGUISH */
/* WHICH DRIVE IS BEING TESTED, AN INTEGER NUMBER IS PASSED AS AN INPUT FOR */
/* THIS FUNCTION. IF THE DIAGNOSTICS ARE SUCCESSFULL, THIS FUNCTION WILL    */
/* RETURN THE EXECUTION TIME FOR THE DIAGNOSTICS, OTHERWISE, IF IT DETECTS  */
/* ANY TYPE OF MALFUNCTION IN A DRIVE, IT WILL RETURN A -1. IF ANY ANOMALY  */
/* IS DETECTED, ERROR CODES ARE SENT TO THE USER AND A FILE, AND IF THERE   */
/* ARE ANY STATUS BYTES ASSOCIATED WITH THE ERROR, THOSE ARE ALSO REPORTED. */
/****************************************************************************/

double disk_check(int drive_id)
{
	int i, testnum, numoflines = 300; 
 	static char writestring[]="abcde"; 
	unsigned disk_status = 0;
	struct diskinfo_t diskinfo;
	char testfile[11], *testbuff, *writebuff, stat[8],timestring[6], errorstring[6];
	long time1, time2;
	FILE *disk_test;

/*  IDENTIFYING WHICH DRIVE IS BEING TESTED. */
	switch (drive_id)
   		{
	    case 1 : strcpy(testfile,FD"test.out");
                     strcpy(timestring,"S015");
                     strcpy(stat,"S21500");
                     strcpy(errorstring,"S1150");
                     testnum = 15; 
			            break;
   		   case 2 : diskinfo.drive = 0;
                     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);
                     if ((disk_status >> 8) == 0x80)
                        {
                           report("S1130010E");
                           tlprintf("Test 13 ERROR, THERE IS NOT A FLOPPY IN THE A DRIVE");
                           return(-1.0);
                        }
                     else
                        if (disk_status != 0x00)
                           {
                              report("S1130011E");
                              tlprintf("Test 13 ERROR OCCURED DURING INITIALIZATION PROCESS");
                              return(-1.0);
                           }
                     strcpy(testfile,"a:test.out");
                     testnum = 13;
                     strcpy(timestring,"S013");
                     strcpy(stat,"S21300");
                     strcpy(errorstring,"S1130");
			            break;
			   case 3 : diskinfo.drive = 1;
                     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);     
                     if ((disk_status >> 8) == 0x80)
                        {
                           report("S1140010E");
                           tlprintf("Test 14 ERROR, THERE IS NOT A FLOPPY IN THE B DRIVE");
                           return(-1.0);
                        }
                     else
                        if (disk_status != 0x00)
                           {
                              report("S1140011E");
                              tlprintf("Test 14 ERROR OCCURED DURING INITIALIZATION PROCESS");
                              return(-1.0);
                           }
                     strcpy(testfile,"b:test.out");
                     strcpy(timestring,"S014");
                     testnum = 14;
                     strcpy(stat,"S21400");
                     strcpy(errorstring,"S1140");
			            break;
         }

/* GENERATING THE CHARACTER STRINGS WHICH ARE USED FOR WRITING INTO A DATA
   FILE AND FOR COMPARING AFTER THE RETRIEVAL FROM A FILE */
  
   if ((writebuff = (char *)malloc(1+numoflines*strlen(writestring))) == NULL)
      {
         reportf("%s001E",errorstring);
         tlprintf("Test %d UNABLE TO ALLOCATE BUFFER SPACE FOR WRITEBUFF.",testnum);
         return(-1.0);
      }
   if ((testbuff = (char *)malloc(1+numoflines*strlen(writestring))) == NULL)
      {
         free(writebuff);
         reportf("%s002E",errorstring);
         tlprintf("Test %d UNABLE TO ALLOCATE BUFFER SPACE FOR TESTBUFF.",testnum);
         return(-1.0);
      }
   strcpy(writebuff,"");
   strcpy(testbuff,"");
 
   /* NUMOFSTRINGLINES IS A GLOBAL VARIABLE SET FROM COMMAND PROMPT LINE */
	for (i = 0; i < numofstringlines; i++)
		strcat(writebuff,writestring);

   /* START OF THE TEST */
	_bios_timeofday(_TIME_GETCLOCK, &time1);
	if ((disk_test = fopen(testfile,"w")) == NULL)
		{
 			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s003E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR OPENING A FILE ON THE DISK FOR WRITING.",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         common_free(writebuff,testbuff);
	      return(-1.0);
		}
	if (fputs(writebuff,disk_test) != 0)
      {
 			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s004E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR WRITING TO FILE ON THE DISK",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         common_free(testbuff,writebuff);
         return(-1.0);
      }
	if (fclose(disk_test) != 0)
      {
 			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s005E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR CLOSING FILE ON DISK",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         common_free(testbuff,writebuff);
         return(-1.0);
      }

	if ((disk_test = fopen(testfile,"r")) == NULL)
		{
 			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s006E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
 		   tlprintf("Test %d ERROR OPENING FILE ON DISK FOR READING",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         common_free(testbuff,writebuff);
		   return(-1.0);
	    }
	if (fgets(testbuff,numoflines*strlen(writestring)+1,disk_test) == NULL)
      {
			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s007E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR RETRIEVING THE STRING FROM THE FILE IN DISK",testnum);
         common_free(testbuff,writebuff);
		   return(-1.0);
	    }
	if (fclose(disk_test) != 0)
      {
			disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
	 reportf("%s005E",errorstring);
	 reportf("%s%.4xE",stat,disk_status);
	 tlprintf("Test %d ERROR CLOSING FILE ON DISK",testnum);
	 tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
	 common_free(writebuff,testbuff);
	 return(-1.0);
      }

	if (strcmp(testbuff,writebuff) != 0)
		{
	 reportf("%s008E",errorstring);
			tlprintf("Test %d THE STRINGS DID NOT MATCH.",testnum);
	 common_free(testbuff,writebuff);
	 return(-1.0);
		}

   common_free(testbuff,writebuff);

	if (remove(testfile))
      {
	 disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
	 reportf("%s009E",errorstring);
	 reportf("%s%.4xE",stat,disk_status);
		tlprintf("Test %d ERROR DELETING THE TEST FILE ON THE DISK.",testnum);
	 tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
	 return(-1.0);
      }

   _bios_timeofday(_TIME_GETCLOCK, &time2);
	reportf("%s%4.2fE",timestring,difftimer(time2,time1));
   tlprintf("Test %d TIME FOR SINGLE DISK TEST WAS %4.2f sec.",testnum,difftimer(time2,time1));

   return(difftimer(time2,time1));
}

/**************************************************************************/
/*                             COMMON_FREE                                */
/* WRITTEN BY LISA PEREIRA                                                */
/*                                                                        */
/* THE PURPOSE OF THIS FUNCTION IS TO FREE THE MEMORY SPACE ALLOCATED TO  */
/* THE CHARACTER STRINGS USED IN THE DISK TESTS. THE INPUTS ARE POINTERS  */
/* TO THE CHARACTER STRINGS.                                              */
/**************************************************************************/

int common_free(char *str1,char *str2)
{
   free(str1);
   free(str2);
   return (1);
}

/****************************************************************************/
/*                            DISK_TO-DISK_CHECK                            */
/* WRITTEN BY LISA PEREIRA                                                  */
/*                                                                          */
/* THIS FUNCTION TESTS THE INTERACTION BETWEEN TWO DISK DRIVES, A AND B, A  */
/* AND C, OR B AND C. IN ORDER TO DISTINGUISH WHICH COMBINATION OF DRIVES   */
/* ARE TO BE TESTED, AN INTEGER NUMBER IS PASSED AS AN INPUT FOR            */
/* THIS FUNCTION. IF THE DIAGNOSTICS ARE SUCCESSFULL, THIS FUNCTION WILL    */
/* RETURN THE EXECUTION TIME FOR THE DIAGNOSTICS, OTHERWISE, IF IT DETECTS  */
/* ANY TYPE OF MALFUNCTION IN A DRIVE, IT WILL RETURN A -1. IF ANY ANOMALY  */
/* IS DETECTED, ERROR CODES ARE SENT TO THE USER AND A FILE, AND IF THERE   */
/* ARE ANY STATUS BYTES ASSOCIATED WITH THE ERROR, THOSE ARE ALSO REPORTED. */
/****************************************************************************/

double disk_to_disk_check (int drives_combo)
{
   static char writestring[]="abcde";
	char testfile2[11],testfile1[11], *testbuff, *writebuff;
   char drive1[2], drive2[2], errorstring[6],timestring[6], stat[8];
	struct diskinfo_t diskinfo;
   unsigned disk_status;
	int testnum, i, numoflines = 300;
	long time1, time2;
	FILE *disk1, *disk2;

   strcpy(drive1,"");
   strcpy(drive2,"");

/* IDENTIFYING THE PAIR OF DRIVES TO BE TESTED. */
	switch (drives_combo)
		{
		     case 1 : diskinfo.drive = 0;
		     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);
		     if ((disk_status >> 8) == 0x80)
			{
			   report("S1170010E");
			   tlprintf("Test 17 ERROR, THERE IS NOT A FLOPPY IN THE A DRIVE");
			   return(-1.0);
			}
		     else
			if (disk_status != 0x00)
			   {
			      report("S1170011E");
			      tlprintf("Test 17 ERROR OCCURED DURING INITIALIZATION PROCESS");
			      return(-1.0);
			   }
		    strcpy(testfile1,"a:test.out");
		    strcpy(testfile2,FD"test.out");
		    strcpy(errorstring,"S1170");
                    strcpy(timestring,"S017");
                    testnum = 17;
                    strcpy(stat,"S21700");
                    strcpy(drive1,"A");
                    strcpy(drive2,"C");
			           break;
   		  case 2 : diskinfo.drive = 0;
                     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);
                     if ((disk_status >> 8) == 0x80)
                        {
                           report("S1160010E");
                           tlprintf("Test 16 ERROR, THERE IS NOT A FLOPPY IN THE A DRIVE");
                           return(-1.0);
                        }
                     else
                        if (disk_status != 0x00)
                           {
                              report("S1160011E");
                              tlprintf("Test 16 ERROR OCCURED DURING INITIALIZATION PROCESS");
                              return(-1.0);
                           }
                    diskinfo.drive = 1;
                     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);
                     if ((disk_status >> 8) == 0x80)
                        {
                           report("S1160010E");
                           tlprintf("Test 16ERROR, THERE IS NOT A FLOPPY IN THE B DRIVE");
                           return(-1.0);
                        }
                     else
                        if (disk_status != 0x00)
                           {
                              report("S1160011E");
                              tlprintf("Test 16 ERROR OCCURED DURING INITIALIZATION PROCESS");
                              return(-1.0);
                           }
                    strcpy(testfile1,"a:test.out");
                    strcpy(testfile2,"b:test.out");
                    strcpy(drive1,"A");
                    testnum = 16;
                    strcpy(errorstring,"S1160");
                    strcpy(timestring,"S016");
                    strcpy(stat,"S21600");   /* Changed from timestring -JCD */
                    strcpy(drive2,"B");
                    break;
			  case 3 : diskinfo.drive = 1;
                     disk_status = _bios_disk(_DISK_VERIFY,&diskinfo);
                     if ((disk_status >> 8) == 0x80)
                        {
                           report("S1180010E");
                           tlprintf("Test 18 ERROR, THERE IS NOT A FLOPPY IN THE B DRIVE");
                           return(-1.0);
                        }
                     else
                        if (disk_status != 0x00)
                           {
                              report("S1180011E");
                              tlprintf("Test 18 ERROR OCCURED DURING INITIALIZATION PROCESS");
                              return(-1.0);
                           }
                    strcpy(testfile1,"b:test.out");
		    strcpy(testfile2,FD"test.out");
                    strcpy(drive1,"B");
                    testnum = 18;
                    strcpy(errorstring,"S1180");
                    strcpy(timestring,"S018");
                    strcpy(stat,"S21800");
                    strcpy(drive2,"C");
			           break;
		}
 /* GENERATING THE CHARACTER STRINGS WHICH ARE USED FOR WRITING INTO A DATA
   FILE AND FOR COMPARING AFTER THE RETRIEVAL FROM A FILE */
 
   if ((writebuff = (char *)malloc(1+numoflines*strlen(writestring))) == NULL)
      {
         reportf("%s001E",errorstring);
         tlprintf("Test %d UNABLE TO ALLOCATE BUFFER SPACE FOR  WRITEBUFF",testnum);
         return(-1.0);
      }
   if ((testbuff = (char *)malloc(1+numoflines*strlen(writestring))) == NULL)
      {
         free(writebuff);
         reportf("%s002E",errorstring);
         tlprintf("Test %d UNABLE TO ALLOCATE BUFFER SPACE FOR TESTBUFF.",testnum);
         return(-1.0);
      }
   strcpy(writebuff,"");
   strcpy(testbuff,"");

   /* NUMOFSTRINGLINES IS A GLOBAL VARIABLE SET BY THE COMMAND LINE */
	for (i = 0; i < numofstringlines; i++)
		strcat(writebuff,writestring);

   /* START OF THE TEST */
   _bios_timeofday(_TIME_GETCLOCK, &time1);
 
	if ((disk1 = fopen(testfile1,"w")) == NULL)
		{
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s003E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR OPENING A FILE ON DRIVE %s FOR WRITING.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         common_free(testbuff,writebuff);
         return(-1.0);
		}

	if (fputs(writebuff,disk1) != 0) 
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s004E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR WRITING TO THE FILE ON DRIVE %s.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
	if (fclose(disk1))
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s005E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR CLOSING THE FILE ON DRIVE %s.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
	if ((disk1 = fopen(testfile1,"r")) == NULL)
		{
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s006E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR OPENING THE FILE ON DRIVE %s TO READ FROM.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
		   return(-1.0);
		}
   if (fgets(testbuff,numoflines*strlen(writestring)+1,disk1) == NULL)
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s007E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR RETRIEVING THE STRING FROM THE FILE",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
   if (fclose(disk1))
      {
         common_free(writebuff,testbuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s005E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR CLOSING THE FILE ON DRIVE %s.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }

   if ((disk2 = fopen(testfile2,"w")) == NULL)
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s003E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR OPENING A FILE ON DRIVE %s FOR WRITING.",testnum,drive2);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
   if (fputs(testbuff,disk2) != 0)
      {
          common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s004E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR WRITING THE STRING TO THE FILE.",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
   if (fclose(disk2))
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s005E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR CLOSING THE FILE ON DRIVE %s AFTER WRITING TO IT.",testnum,drive2);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
   if ((disk2 = fopen(testfile2,"r")) == NULL)
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s006E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR OPENING THE FILE ON DRIVE %s FOR READING.",testnum,drive2);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }             
   if (fgets(testbuff,numoflines*strlen(writestring)+1,disk2) == NULL)
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s007E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR RETRIEVING THE STRING FROM THE FILE",testnum);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
   if (fclose(disk2))
      {
         common_free(testbuff,writebuff);
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s005E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
         tlprintf("Test %d ERROR CLOSING THE FILE ON DRIVE %s.",testnum,drive2);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
	if (strcmp(testbuff,writebuff) != 0)
	 	{
         reportf("%s008E",errorstring);
			tlprintf("Test %d THE STRINGS DID NOT MATCH.",testnum);
         common_free(testbuff,writebuff);
         return(-1.0);
		}
	if (remove(testfile1))
      {
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s009E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
   		tlprintf("Test %d ERROR DELETING THE TEST FILE ON DRIVE %s.",testnum,drive1);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }
 	if (remove(testfile2))
      {
         disk_status = _bios_disk(_DISK_STATUS,&diskinfo);
         reportf("%s009E",errorstring);
         reportf("%s%.4xE",stat,disk_status);
   		tlprintf("Test %d ERROR DELETING THE TEST FILE ON DRIVE %s.",testnum,drive2);
         tlprintf("TEST %d DISK STATUS BYTE : %.4x",testnum,disk_status);
         return(-1.0);
      }

   _bios_timeofday(_TIME_GETCLOCK, &time2);
   common_free(testbuff,writebuff);
	reportf("%s%4.2fE",timestring,difftimer(time2,time1));
   tlprintf("Test %d TIME FOR THE TWO DISKS TEST WAS %4.2f sec.",testnum,difftimer(time2,time1));

   return(difftimer(time2,time1));
}

/************************************************************************/
/*                                PRINTER_TEST                          */
/* WRITTEN BY LISA PEREIRA                                              */
/*                                                                      */
/* THIS FUNCTION TESTS THE STATUS OF THE PRINTER BY INVOKING THE BIOS   */
/* COMMAND. SHOULD THE STATUS OF THE PRINTER CHANGE, OR A FAULT BE      */
/* DETECTED THE STATUS BYTE AND/OR ERROR CODE ARE SENT TO THE USER, IF  */
/* NOT, THEN THE EXECUTION TIME IS SENT.                                */
/************************************************************************/

double printer_test(int x)
{
   static unsigned old_printer_stat;
   unsigned printer_stat, temp_pri_stat = 0, dummy;
   long time1, time2;
   int  flag = 1;

      /* START OF THE TEST */
         _bios_timeofday(_TIME_GETCLOCK, &time1);
         printer_stat = _bios_printer(_PRINTER_STATUS,0,dummy);
         if (printer_stat != old_printer_stat)
            {
               temp_pri_stat = printer_stat;
               _bios_timeofday(_TIME_GETCLOCK, &time2);
               if (printer_stat & 0001)
                  {
                     flag = 0;
                     tlprintf("TEST 05 ERROR, PRINTER TIMED OUT.");
                     report("S1050001E");
                  }
               if ((printer_stat >> 3) & 0001)
                  {
                     report("S1050002E");
                     tlprintf("TEST 05 PRINTER I/O ERROR.");
                     flag = 0;
                  }
            }
         else
            _bios_timeofday(_TIME_GETCLOCK, &time2);
         if (flag == 1)
            {
               reportf("S005%4.2fE",difftimer(time2,time1));
               tlprintf("TEST 05 PRINTER TEST EXECUTION TIME : %4.2f sec",difftimer(time2,time1));
            }
         if (temp_pri_stat != 0)
            {
               reportf("S20500%.4xE",temp_pri_stat);         
               tlprintf("TEST 05 PRINTER STATUS BYTE : %.4X.",temp_pri_stat);
            }

/* GETTING THE ORIGINAL PRINTER STATUS DURING CALIBRATION */
   if (reset_status_bytes)
      old_printer_stat = _bios_printer(_PRINTER_STATUS,0,dummy);

   return(difftimer(time2,time1));
}
/**************************************************************************/
/*                       ENV_TEST                                         */
/* WRITTEN BY LISA PEREIRA                                                */
/*                                                                        */
/* THIS FUNCTION CHECKS TO SEE IF THE ENVIRONMENT HAS BEEN CORRUPTED. IT  */
/* DETERMINES THE STATUS OF THE ENVIRONMENT BY COMPARING THE CURRENT      */
/* ENVIRONMENT VARIABLES WITH THE ENVIRONMENT VARIABLES STORED DURING THE */
/* CALIBRATION RUN.                                                       */
/**************************************************************************/

double env_test(int x)
{
   int i = 0;
   static char *temp_env[50];
   long time1, time2;

/* GETTING THE ORIGINAL ENVIRONMENT VARIABLES DURING THE CALIBRATION */
   if (reset_status_bytes)
   {  for (i = 0; environ[i] != NULL; i++)
         temp_env[i] = environ[i];
   }

/* START OF THE TEST */
   _bios_timeofday(_TIME_GETCLOCK, &time1);
   for (i = 0; environ[i] != NULL; i++)
      if (strcmp(temp_env[i],environ[i]) != 0)
         {
            report("S1090001E");
            tlprintf("Test 09 THE ENVIRONMENT VARIABLE %s HAS BEEN CORRUPTED.",environ[i]);
            return(-1.0);
         }
   _bios_timeofday(_TIME_GETCLOCK, &time2);
   reportf("S009%4.2fE",difftimer(time2,time1));
   tlprintf("Test 09 TIME TO ACCESS AND TEST THE ENVIRONMENT WAS %4.2f sec.",difftimer(time2,time1));

   return (difftimer(time2,time1));
}

/**************************************************************************/
/*                       VIDEO_TEST                                       */
/* WRITTEN BY LISA PEREIRA                                                */
/*                                                                        */
/* THIS FUNCTION TESTS VARIOUS ASPECTS THAT DEAL WITH A COMPUTERS VIDEO.  */
/* THE FIRST TEST INVOLVES WRITING A STRING DIRECTLY TO THE CONSOLE. THEN */
/* SEVERAL TESTS ARE PERFORMED THAT DEAL WITH THE GRAPHICS CAPABILITIES   */
/* OF THE COMPUTER. THESE TESTS INVOLVING THE ABILITY TO SWITCH BETWEEN   */
/* ACTIVE AND VISUAL PAGES, AS WELL AS DRAWING RECTANGLES WITH DIFFERENT  */
/* COLORS. SHOULD ANY ERROR OCCUR, THE USER IS NOTIFIED WITH SPECIFIC     */
/* ERROR CODES.                                                           */
/**************************************************************************/

double test_video(int x)
{
   int i,colornumber = 0;
   long time1,time2;
   BOOLEAN found_error = FALSE;

   /* START OF TEST */ 
   _bios_timeofday(_TIME_GETCLOCK, &time1);

   /* TESTING THE CONSOLE */
   for (i = 1; i <= 20 && (!b_wnerr); i++)
      if (cputs("THE VIDEO IS CURRENTLY BEING TESTED.\n") != 0)
      {
         report("S1010001E");
         found_error = TRUE;
         tlprintf("TEST 01 ERROR WRITING TO THE SCREEN.");
         break;
      }   

   if (!found_error)         /* TESTING THE GRAPHICS CAPABILITIES. */
   {
      if (_setvideomode(_MRES4COLOR) == 0)
         if (_setvideomode(_MRESNOCOLOR) == 0)
            {
               report("S101002E");
               found_error = TRUE;
               tlprintf("TEST 01 ERROR SETTING COLOR RESOLUTION.");
            }

      /* Color monitor present. */

      if (!found_error)
      {
         if (_setvisualpage(0) < 0)
         {
            tlprintf("TEST 01 ERROR SETTING THE VISUAL PAGE.");
            found_error = TRUE;
            report("S1010005E");
         }

         if ((!found_error) && (_setactivepage(0) < 0))
         {
            found_error = TRUE;
            report("S1010003E");
            tlprintf("TEST 01 ERROR SETTING THE ACTIVE VIDEO PAGE.");
         }

         for (i = 0; i < 320; i = i + 40)
         {
            _setcolor(colornumber%4);
            colornumber++;
            if ((!found_error) && (_rectangle(_GFILLINTERIOR,i,10,i+40,200) == 0))
            {
              report("S1010004E");
              tlprintf("TEST 01 ERROR DRAWING THE RECTANGLE.");
              found_error = TRUE;
              break;
            }
         }

         /* VIDEO_TIME IS A GLOBAL SET FROM THE COMMAND LINE */
         utsleep(video_time);
      }
   } /* IF THERE WAS NO ERROR IN CPUTS */

   _setvideomode(_DEFAULTMODE);
   _bios_timeofday(_TIME_GETCLOCK, &time2);

   if (!found_error)
   {
      reportf("S001%4.2fE",difftimer(time2,time1));
      tlprintf("TEST 01 TIME FOR VIDEO TEST WAS %4.2f sec.",difftimer(time2,time1));
   }

   if (!calibration_run)
      assert(wnredraw(global_device, act_page)==WN_NO_ERROR);

   return (difftimer(time2,time1));
}

/***************************************************************************/
/*                             HEAP_TEST                                   */
/* WRITTEN BY LISA PEREIRA                                                 */
/*                                                                         */
/* THIS FUNCTION TEST THE CONSISTENCY OF THE HEAP (MEMORY). IT FIRST       */
/* ALLOCATES THE AMOUNT OF MEMORY AVAILABLE, THEN THE HEAP'S STATUS IS     */
/* CHECKED WITH THE HEAPSET COMMAND, WHICH WILL ALSO WRITE INTO THE FREE   */
/* HEAP AREAS. ONCE THE DIAGNOSTICS IS DONE, THE MEMORY ALLOCATED IS FREED */
/***************************************************************************/

double heap_test(int x)
{
   size_t amount_of_mem_to_alloc = _memavl();
   int heap_status;
   char *dummyptr;
   long time1,time2;

/* START OF TEST */
   _bios_timeofday(_TIME_GETCLOCK, &time1);
   dummyptr = (char *)malloc(amount_of_mem_to_alloc);
   heap_status = _heapset(3);
   switch (heap_status)
      {
         case _HEAPEMPTY    : report("S1120001E");
                              tlprintf("TEST 12 ERROR IDENTIFYING A FULL HEAP AS EMPTY");
                              free(dummyptr);
                              return(-1.0);
         case _HEAPBADBEGIN : report("S1120002E");
                              free(dummyptr);
                              tlprintf("TEST 12 INITIAL HEADER INFORMATION FOR THE HEAP NOT FOUND");
                              return(-1.0);
         case _HEAPBADNODE  : report("S1120003E");
                              tlprintf("TEST 12 DAMAGED HEAP OR BAD NODE FOUND");
                              free(dummyptr);   
                              return(-1.0); 

       }
   _bios_timeofday(_TIME_GETCLOCK, &time2);
   reportf("S012%4.2fE",difftimer(time2,time1));
   tlprintf("TEST 12 TIME FOR ACCESSING AND TESTING THE HEAP WAS %4.2f sec.",difftimer(time2,time1));
   free(dummyptr);

   return(difftimer(time2,time1));
}
