/*-----------------------------------------------------------
   HDRLIB.C -- "Handy Drawing Routines" Dynamic Link Library
               (c) Charles Petzold, 1993
  -----------------------------------------------------------*/

#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "hdrlib.h"

INT APIENTRY HdrPuts (HPS hps, PPOINTL pptl, PCHAR szText)
     {
     INT iLength = strlen (szText) ;

     if (pptl == NULL)
          GpiCharString (hps, iLength, szText) ;
     else
          GpiCharStringAt (hps, pptl, iLength, szText) ;

     return iLength ;
     }

INT APIENTRY HdrPrintf (HPS hps, PPOINTL pptl, PCHAR szFormat, ...)
     {
     static CHAR chBuffer [1024] ;
     INT         iLength ;
     va_list     pArguments ;

     va_start (pArguments, szFormat) ;
     iLength = vsprintf (chBuffer, szFormat, pArguments) ;

     if (pptl == NULL)
          GpiCharString (hps, iLength, chBuffer) ;
     else
          GpiCharStringAt (hps, pptl, iLength, chBuffer) ;

     va_end (pArguments) ;
     return iLength ;
     }

LONG APIENTRY HdrEllipse (HPS hps, LONG lOption, PPOINTL pptl)
     {
     POINTL ptlCurrent ;

     GpiQueryCurrentPosition (hps, &ptlCurrent) ;

     return GpiBox (hps, lOption, pptl, labs (pptl->x - ptlCurrent.x),
                                        labs (pptl->y - ptlCurrent.y)) ;
     }
