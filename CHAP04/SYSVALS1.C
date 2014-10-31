/*---------------------------------------------------
   SYSVALS1.C -- System Values Display Program No. 1
                 (c) Charles Petzold, 1993
  ---------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "sysvals.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "SysVals1" ;
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
     static INT  cxChar, cxCaps, cyChar, cyDesc, cyClient ;
     CHAR        szBuffer [10] ;
     FONTMETRICS fm ;
     HPS         hps ;
     INT         iLine ;
     POINTL      ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;

               cxChar = fm.lAveCharWidth ;
               cxCaps = (fm.fsType & 1 ? 2 : 3) * cxChar / 2 ;
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

               for (iLine = 0 ; iLine < NUMLINES ; iLine++)
                    {
                    ptl.x = cxCaps ;
                    ptl.y = cyClient - cyChar * (iLine + 1) + cyDesc ;

                    GpiCharStringAt (hps, &ptl,
                              strlen (sysvals[iLine].szIdentifier),
                              sysvals[iLine].szIdentifier) ;

                    ptl.x += 24 * cxCaps ;
                    GpiCharStringAt (hps, &ptl,
                                     strlen (sysvals[iLine].szDescription),
                                     sysvals[iLine].szDescription) ;

                    sprintf (szBuffer, "%d",
                             WinQuerySysValue (HWND_DESKTOP,
                                               sysvals[iLine].sIndex)) ;

                    ptl.x += 38 * cxChar ;
                    GpiCharStringAt (hps, &ptl, strlen (szBuffer),
                                     szBuffer) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
