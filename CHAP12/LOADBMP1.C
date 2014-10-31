/*----------------------------------------------------
   LOADBMP1.C -- Loads a Bitmap Resource and Draws it
                 (c) Charles Petzold, 1993
  ----------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include "loadbmp.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "LoadBmp1" ;
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
     static INT cxClient, cyClient ;
     HBITMAP    hbm ;
     HPS        hps ;
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

               hbm = GpiLoadBitmap (hps, 0, IDB_HELLO, cxClient, cyClient) ;

               if (hbm)
                    {
                    ptl.x = 0 ;
                    ptl.y = 0 ;

                    WinDrawBitmap (hps, hbm, NULL, &ptl,
                                   CLR_BACKGROUND, CLR_NEUTRAL, DBM_NORMAL) ;

                    GpiDeleteBitmap (hbm) ;               
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
