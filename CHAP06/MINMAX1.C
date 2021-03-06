/*-----------------------------------------------
   MINMAX1.C -- Bitblt of Minimize-Maximize Menu
                (c) Charles Petzold, 1993
  -----------------------------------------------*/

#define INCL_WIN
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "MinMax1" ;
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
     static INT  cxClient, cyClient ;
     static LONG cxMinMax, cyMinMax ;
     HPS         hps ;
     POINTL      aptl[3] ;
     INT         iRow, iCol ;

     switch (msg)
          {
          case WM_CREATE:
               cxMinMax = WinQuerySysValue (HWND_DESKTOP, SV_CXMINMAXBUTTON) ;
               cyMinMax = WinQuerySysValue (HWND_DESKTOP, SV_CYMINMAXBUTTON) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               GpiErase (hps) ;

               for (iRow = 0 ; iRow <= cyClient / cyMinMax ; iRow++)
                    for (iCol = 0 ; iCol <= cxClient / cxMinMax ; iCol++)
                         {
                         aptl[0].x = iCol * cxMinMax ;      // target
                         aptl[0].y = iRow * cyMinMax ;      //   lower left

                         aptl[1].x = aptl[0].x + cxMinMax ; // target
                         aptl[1].y = aptl[0].y + cyMinMax ; //   upper right

                         aptl[2].x = cxClient - cxMinMax ;  // source
                         aptl[2].y = cyClient ;             //   lower left

                         GpiBitBlt (hps, hps, 3L, aptl, ROP_SRCCOPY, BBO_AND) ;
                         }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
