/*---------------------------------------
   BEZIER.C -- Bezier Splines
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Bezier" ;
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
     static POINTL aptl[4] ;
     HPS           hps ;
     INT           cxClient, cyClient ;

     switch (msg)
          {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               aptl[0].x = cxClient / 3 ;
               aptl[0].y = cyClient / 2 ;

               aptl[1].x = cxClient / 2 ;
               aptl[1].y = 3 * cyClient / 4 ;

               aptl[2].x = cxClient / 2 ;
               aptl[2].y = cyClient / 4 ;

               aptl[3].x = 2 * cxClient / 3 ;
               aptl[3].y = cyClient / 2 ;

               return 0 ;

          case WM_BUTTON1DOWN:
               aptl[1].x = MOUSEMSG(&msg)->x ;
               aptl[1].y = MOUSEMSG(&msg)->y ;

               WinInvalidateRect (hwnd, NULL, TRUE) ;
               break ;

          case WM_BUTTON2DOWN:
               aptl[2].x = MOUSEMSG(&msg)->x ;
               aptl[2].y = MOUSEMSG(&msg)->y ;

               WinInvalidateRect (hwnd, NULL, TRUE) ;
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

                    // Draw dotted straight lines

               GpiSetLineType (hps, LINETYPE_DOT) ;
               GpiMove (hps, aptl + 0) ;
               GpiLine (hps, aptl + 1) ;
               GpiMove (hps, aptl + 2) ;
               GpiLine (hps, aptl + 3) ;

                    // Draw spline

               GpiSetLineType (hps, LINETYPE_SOLID) ;
               GpiMove (hps, aptl) ;
               GpiPolySpline (hps, 3, aptl + 1) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
