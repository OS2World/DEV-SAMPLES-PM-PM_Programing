/*-----------------------------------------
   PATTERNS.C -- GPI Area Patterns
                 (c) Charles Petzold, 1993
  -----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Patterns" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, 0, 0, &hwndClient) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static struct {
                   LONG lPatternSymbol ;
                   CHAR *szPatternSymbol ;
                   }
                   show [] = 
                   {
                   PATSYM_DEFAULT   , "PATSYM_DEFAULT"   ,
                   PATSYM_DENSE1    , "PATSYM_DENSE1"    ,
                   PATSYM_DENSE2    , "PATSYM_DENSE2"    ,
                   PATSYM_DENSE3    , "PATSYM_DENSE3"    ,
                   PATSYM_DENSE4    , "PATSYM_DENSE4"    ,
                   PATSYM_DENSE5    , "PATSYM_DENSE5"    ,
                   PATSYM_DENSE6    , "PATSYM_DENSE6"    ,
                   PATSYM_DENSE7    , "PATSYM_DENSE7"    ,
                   PATSYM_DENSE8    , "PATSYM_DENSE8"    ,
                   PATSYM_VERT      , "PATSYM_VERT"      ,
                   PATSYM_HORIZ     , "PATSYM_HORIZ"     ,
                   PATSYM_DIAG1     , "PATSYM_DIAG1"     ,
                   PATSYM_DIAG2     , "PATSYM_DIAG2"     ,
                   PATSYM_DIAG3     , "PATSYM_DIAG3"     ,
                   PATSYM_DIAG4     , "PATSYM_DIAG4"     ,
                   PATSYM_NOSHADE   , "PATSYM_NOSHADE"   ,
                   PATSYM_SOLID     , "PATSYM_SOLID"     ,
                   PATSYM_HALFTONE  , "PATSYM_HALFTONE"  ,
                   PATSYM_HATCH     , "PATSYM_HATCH"     ,
                   PATSYM_DIAGHATCH , "PATSYM_DIAGHATCH" ,
                   PATSYM_BLANK     , "PATSYM_BLANK"
                   } ;
     static INT    cyClient, cxCaps, cyChar, cyDesc,
                   iNumTypes = sizeof show / sizeof show[0] ;
     FONTMETRICS   fm ;
     HPS           hps ;
     INT           i ;
     POINTL        ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxCaps = (fm.fsType & 1 ? 2 : 3) * fm.lAveCharWidth / 2 ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;
               WinReleasePS (hps) ;
               return 0 ;

          case WM_SIZE:
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               for (i = 0 ; i < iNumTypes ; i ++)
                    {
                    GpiSetPattern (hps, show [i].lPatternSymbol) ;

                    ptl.x = (i < 11 ? 1 : 33) * cxCaps ;
                    ptl.y = cyClient - (i % 11 * 5 + 4) * cyChar / 2 + cyDesc ;

                    GpiCharStringAt (hps, &ptl,
                                     strlen (show [i].szPatternSymbol),
                                     show [i].szPatternSymbol) ;

                    ptl.x  = (i < 11 ? 20 : 52) * cxCaps ;
       	            ptl.y -= cyDesc + cyChar / 2 ;
                    GpiMove (hps, &ptl) ;

                    ptl.x += 10 * cxCaps ;
                    ptl.y +=  2 * cyChar ;
                    GpiBox (hps, DRO_FILL, &ptl, 0L, 0L) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
