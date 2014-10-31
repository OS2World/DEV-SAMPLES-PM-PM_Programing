/*--------------------------------------------------
   DEVCAPS.C -- Device Capabilities Display Program
                (c) Charles Petzold, 1993
  --------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "devcaps.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
LONG             RtJustCharStringAt (HPS, PPOINTL, LONG, PCHAR) ;

int main (void)
     {
     static CHAR  szClientClass [] = "DevCaps" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, 0L, 0) ;

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
     static HDC  hdc ;
     static INT  cyClient, cxCaps, cyChar, cyDesc ;
     CHAR        szBuffer [12] ;
     FONTMETRICS fm ;
     HPS         hps ;
     INT         i ;
     LONG        lValue ;
     POINTL      ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxCaps = fm.lEmInc ;
               cxCaps = (fm.fsType & 1 ? 2 : 3) * fm.lAveCharWidth / 2 ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;
               WinReleasePS (hps) ;

               hdc = WinOpenWindowDC (hwnd) ;
               return 0 ;

          case WM_SIZE:
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               for (i = 0 ; i < NUMLINES ; i++)
                    {
                    ptl.x = cxCaps ;
                    ptl.y = cyClient - cyChar * (i + 2) + cyDesc ;

                    if (i >= (NUMLINES + 1) / 2)
                         {
                         ptl.x += cxCaps * 35 ;
                         ptl.y += cyChar * ((NUMLINES + 1) / 2) ;
                         }

                    DevQueryCaps (hdc, devcaps[i].lIndex, 1L, &lValue) ;

                    GpiCharStringAt (hps, &ptl,
                                     strlen (devcaps[i].szIdentifier),
                                     devcaps[i].szIdentifier) ;

                    ptl.x += 33 * cxCaps ;
                    RtJustCharStringAt (hps, &ptl,
                              sprintf (szBuffer, "%d", lValue),
                              szBuffer) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

LONG RtJustCharStringAt (HPS hps, PPOINTL pptl, LONG lLength, PCHAR pchText)
     {
     POINTL aptlTextBox[TXTBOX_COUNT] ;

     GpiQueryTextBox (hps, lLength, pchText, TXTBOX_COUNT, aptlTextBox) ;

     pptl->x -= aptlTextBox[TXTBOX_CONCAT].x ;

     return GpiCharStringAt (hps, pptl, lLength, pchText) ;
     }
