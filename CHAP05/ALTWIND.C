/*------------------------------------------
   ALTWIND.C -- Alternate and Winding Modes
                (c) Charles Petzold, 1993
 -------------------------------------------*/

#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "AltWind" ;
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
     static POINTL aptlFigure[10] = { 10,30, 50,30, 50,90, 90,90, 90,50,
                                      30,50, 30,10, 70,10, 70,70, 10,70 } ;
     static INT    cxClient, cyClient ;
     HPS           hps ;
     INT           i ;
     POINTL        aptl[10] ;

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

               for (i = 0 ; i < 10 ; i++)
                    {
                    aptl[i].x = cxClient * aptlFigure[i].x / 200 ;
                    aptl[i].y = cyClient * aptlFigure[i].y / 100 ;
                    }

               GpiBeginArea (hps, BA_BOUNDARY | BA_ALTERNATE) ;
               GpiMove (hps, aptl) ;
               GpiPolyLine (hps, 9L, aptl + 1) ;
               GpiEndArea (hps) ;

                         /*-------------------
                            Winding Fill Mode
                           -------------------*/

               for (i = 0 ; i < 10 ; i++)
                    aptl[i].x += cxClient / 2 ;

               GpiBeginArea (hps, BA_BOUNDARY | BA_WINDING) ;
               GpiMove (hps, aptl) ;
               GpiPolyLine (hps, 9L, aptl + 1) ;
               GpiEndArea (hps) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
