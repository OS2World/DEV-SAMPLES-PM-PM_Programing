/*--------------------------------------
   RULER.C -- Draw a Ruler
              (c) Charles Petzold, 1993
  --------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Ruler" ;
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
     static INT   iTick[16] = { 100, 25, 35, 25, 50, 25, 35, 25,
                                 70, 25, 35, 25, 50, 25, 35, 25 } ;
     static INT   cxClient, cxChar, cyDesc ;
     static SIZEL sizl ;
     CHAR         szBuffer [4] ;
     FONTMETRICS  fm ;
     HPS          hps ;
     INT          i ;
     POINTL       ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               GpiSetPS (hps, &sizl, PU_LOENGLISH) ;

               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyDesc = fm.lMaxDescender ;

               WinReleasePS (hps) ;
               return 0 ;

          case WM_SIZE:
               ptl.x = SHORT1FROMMP (mp2) ;
               ptl.y = SHORT2FROMMP (mp2) ;

               hps = WinGetPS (hwnd) ;
               GpiSetPS (hps, &sizl, PU_LOENGLISH) ;
               GpiConvert (hps, CVTC_DEVICE, CVTC_PAGE, 1L, &ptl) ;
               WinReleasePS (hps) ;

               cxClient = ptl.x ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiSetPS (hps, &sizl, PU_LOENGLISH) ;
               GpiErase (hps) ;

               for (i = 0 ; i < 16 * cxClient / 100 ; i++)
                    {
                    ptl.x = 100 * i / 16 ;
                    ptl.y = 0 ;
                    GpiMove (hps, &ptl) ;

                    ptl.y = iTick [i % 16] ;
                    GpiLine (hps, &ptl) ;

                    if (i % 16 == 0)
                         {
                         ptl.x -= cxChar / (i >= 160 ? 1 : 2) ;
                         ptl.y += cyDesc ;
                         GpiCharStringAt (hps, &ptl,
                                          sprintf (szBuffer, "%d", i / 16),
                                          szBuffer) ;
                         }
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
