/*------------------------------------------------------------------
   PRINTDC.C -- Function to open device context for default printer
                (c) Charles Petzold, 1993
  ------------------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include <string.h>
#pragma pack(1)          // align structure fields on 1-byte boundaries

HDC OpenDefaultPrinterDC (HAB hab)
     {
     static CHAR     achPrnData[256] ;
     static DRIVDATA driv = { sizeof (DRIVDATA) } ;
     CHAR            achDefPrnName[33], *pchDelimiter ;
     DEVOPENSTRUC    dop ;

               // Obtain default printer name and remove semicolon

     PrfQueryProfileString (HINI_PROFILE, "PM_SPOOLER",
                            "PRINTER", ";",
                            achDefPrnName, sizeof achDefPrnName) ;

     if ((pchDelimiter = strchr (achDefPrnName, ';')) != NULL)
          *pchDelimiter = '\0' ;

     if (achDefPrnName[0] == '\0')
          return DEV_ERROR ;

               // Obtain information on default printer

     PrfQueryProfileString (HINI_PROFILE, "PM_SPOOLER_PRINTER",
                            achDefPrnName, ";;;;",
                            achPrnData, sizeof achPrnData) ;

               // Parse printer information string

     if ((pchDelimiter = strchr (achPrnData, ';')) == NULL)
          return DEV_ERROR ;

     dop.pszDriverName = pchDelimiter + 1 ;

     if ((pchDelimiter = strchr (dop.pszDriverName, ';')) == NULL)
          return DEV_ERROR ;

     dop.pszLogAddress = pchDelimiter + 1 ;

     *(dop.pszLogAddress + strcspn (dop.pszLogAddress, ",;")) = '\0' ;
     *(dop.pszDriverName + strcspn (dop.pszDriverName, ",;")) = '\0' ;

               // Fill DRIVDATA structure if necessary

     if ((pchDelimiter = strchr (dop.pszDriverName, '.')) != NULL)
          {
          *pchDelimiter = '\0' ;
          strncpy (driv.szDeviceName, pchDelimiter + 1,
                                      sizeof (driv.szDeviceName)) ;
          dop.pdriv = &driv ;
          }
     else
          dop.pdriv = NULL ;

               // Set data type to "std"

     dop.pszDataType = "PM_Q_STD" ;

               // Open printer device context

     return DevOpenDC (hab, OD_QUEUED, "*", 4L, (PDEVOPENDATA) &dop, 0L) ;
     }
