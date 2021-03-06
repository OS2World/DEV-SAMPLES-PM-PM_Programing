/*--------------------------------------------------------------
   HDRTEST.C -- Program to Test HDRLIB.DLL Dynamic Link Library
                (c) Charles Petzold, 1993
  --------------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include "hdrlib.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "HdrTest" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB            hab ;
     HMQ            hmq ;
     HWND           hwndFrame, hwndClient ;
     QMSG           qmsg ;

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
     static INT cxClient, cyClient ;
     HPS        hps;
     POINTL     ptl ;

     switch (msg)
	  {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               ptl.x = cxClient / 8 ;
               ptl.y = 3 * cyClient / 4 ;
               HdrPrintf (hps, &ptl, "Welcome to the %s",
                          "OS/2 2.0 Presentation Manager!") ;

               ptl.x = cxClient / 8 ;
               ptl.y = cyClient / 4 ;
               HdrPuts (hps, &ptl, "This line was displayed by a ") ;
               HdrPuts (hps, NULL, "routine in a dynamic link library.") ;

               ptl.x = 0 ;
               ptl.y = 0 ;
               GpiMove (hps, &ptl) ;

               ptl.x = cxClient - 1 ;
               ptl.y = cyClient - 1 ;
               HdrEllipse (hps, DRO_OUTLINE, &ptl) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
