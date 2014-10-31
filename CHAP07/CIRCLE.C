/*---------------------------------------
   CIRCLE.C -- Transform Demonstration
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#define min(a,b)    ((a) < (b) ? (a) : (b))

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Circle" ;
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

void DrawFigure (HPS hps)
     {
     POINTL ptl ;

     ptl.x = -1000 ;
     ptl.y = -1000 ;
     GpiMove (hps, &ptl) ;

     ptl.x = 1000 ;
     ptl.y = 1000 ;
     GpiBox (hps, DRO_OUTLINE, &ptl, 2000, 2000) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static INT cxClient, cyClient ;
     HPS        hps ;
     POINTL     ptl ;
     RECTL      rcl ;
     SIZEL      sizel ;

     switch (msg)
          {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

                    // Draw ellipse

               sizel.cx = 1000 ;
               sizel.cy = 1000 ;
               GpiSetPS (hps, &sizel, PU_PELS) ;

               rcl.xLeft   = cxClient / 2 ;
               rcl.xRight  = cxClient ;
               rcl.yBottom = cyClient / 2 ;
               rcl.yTop    = cyClient ;

               GpiSetPageViewport (hps, &rcl) ;

               DrawFigure (hps) ;

                    // Draw circle

               sizel.cx = 1000 ;
               sizel.cy = 1000 ;
               GpiSetPS (hps, &sizel, PU_ARBITRARY) ;

               ptl.x = cxClient ;
               ptl.y = cyClient ;

               GpiConvert (hps, CVTC_DEVICE, CVTC_PAGE, 1, &ptl) ;

               ptl.x = ptl.y = min (ptl.x, ptl.y) ;

               GpiConvert (hps, CVTC_PAGE, CVTC_DEVICE, 1, &ptl) ;

               rcl.xLeft   = cxClient / 2 ;
               rcl.xRight  = (ptl.x + cxClient) / 2 ;
               rcl.yBottom = cyClient / 2 ;
               rcl.yTop    = (ptl.y + cyClient) / 2 ;

               GpiSetPageViewport (hps, &rcl) ;

               DrawFigure (hps) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
