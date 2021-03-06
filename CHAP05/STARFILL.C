/*--------------------------------------------
   STARFILL.C -- Alternate and Winding Modes
                    (c) Charles Petzold, 1993
  --------------------------------------------*/

#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "StarFill" ;
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
     static POINTL aptlStar[5] = {-59,-81, 0,100, 59,-81, -95,31, 95,31 } ;
     static INT    cxClient, cyClient ;
     HPS           hps ;
     INT           i ;
     POINTL        aptl[5] ;

     switch (msg)
	  {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;
               GpiSetPattern (hps, PATSYM_HALFTONE) ;

                         /*---------------------
                            Alternate Fill Mode
                           ---------------------*/

               for (i = 0 ; i < 5 ; i++)
                    {
                    aptl[i].x = cxClient / 4 + cxClient *
                                          aptlStar[i].x / 400 ;
                    aptl[i].y = cyClient / 2 + cyClient *
                                          aptlStar[i].y / 200 ;
                    }

               GpiBeginArea (hps, BA_NOBOUNDARY | BA_ALTERNATE) ;
               GpiMove (hps, aptl) ;
               GpiPolyLine (hps, 4L, aptl + 1) ;
               GpiEndArea (hps) ;

                         /*-------------------
                            Winding Fill Mode
                           -------------------*/

               for (i = 0 ; i < 5 ; i++)
                    aptl[i].x += cxClient / 2 ;

               GpiBeginArea (hps, BA_NOBOUNDARY | BA_WINDING) ;
               GpiMove (hps, aptl) ;
               GpiPolyLine (hps, 4L, aptl + 1) ;
               GpiEndArea (hps) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
