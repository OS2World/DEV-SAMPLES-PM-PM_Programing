/*--------------------------------------
   PATHS.C -- Path Functions
              (c) Charles Petzold, 1993
  --------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Paths" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
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
     HPS         hps ;
     INT         x, y ;
     POINTL      ptl ;

     switch (msg)
	  {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               GpiErase (hps) ;

               for (x = 0 ; x < 3 ; x++)
               for (y = 0 ; y < 2 ; y++)
                    {
                         // Create an open sub-path

                    GpiBeginPath (hps, 1) ;

                    ptl.x = (1 + 10 * x) * cxClient / 30 ;
                    ptl.y = (4 +  5 * y) * cyClient / 10 ;
                    GpiMove (hps, &ptl) ;

                    ptl.x = (5 + 10 * x) * cxClient / 30 ;
                    ptl.y = (2 +  5 * y) * cyClient / 10 ;
                    GpiLine (hps, &ptl) ;

                    ptl.x = (9 + 10 * x) * cxClient / 30 ;
                    ptl.y = (4 +  5 * y) * cyClient / 10 ;
                    GpiLine (hps, &ptl) ;

                         // Create a closed sub-path

                    ptl.x = (1 + 10 * x) * cxClient / 30 ;
                    ptl.y = (3 +  5 * y) * cyClient / 10 ;
                    GpiMove (hps, &ptl) ;

                    ptl.x = (5 + 10 * x) * cxClient / 30 ;
                    ptl.y = (1 +  5 * y) * cyClient / 10 ;
                    GpiLine (hps, &ptl) ;

                    ptl.x = (9 + 10 * x) * cxClient / 30 ;
                    ptl.y = (3 +  5 * y) * cyClient / 10 ;
                    GpiLine (hps, &ptl) ;

                    GpiCloseFigure (hps) ;
                    GpiEndPath (hps) ;

                         // Possibly modify the path

                    if (y == 0)
                         {
                         GpiSetLineWidthGeom (hps, cxClient / 30) ;
                         GpiModifyPath (hps, 1, MPATH_STROKE) ;
                         }

                         // Perform the operation

                    GpiSetLineWidth (hps, LINEWIDTH_THICK) ;
                    GpiSetLineWidthGeom (hps, cxClient / 50) ;
                    GpiSetPattern (hps, PATSYM_HALFTONE) ;

                    switch (x)
                         {
                         case 0:  GpiOutlinePath (hps, 1, 0) ;
                                  break ;

                         case 1:  GpiStrokePath (hps, 1, 0) ;
                                  break ;

                         case 2:  GpiFillPath (hps, 1, FPATH_ALTERNATE) ;
                                  break ;
                         }
                    }

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
