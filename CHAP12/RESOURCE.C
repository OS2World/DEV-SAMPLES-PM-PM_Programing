/*-------------------------------------------------
   RESOURCE.C -- Uses an Icon and Pointer Resource
                 (c) Charles Petzold, 1993
  -------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include "resource.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Resource" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_ICON ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, 0, ID_RESOURCE, &hwndClient) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static HPOINTER hIcon, hptr ;
     static INT      cxClient, cyClient, cxIcon, cyIcon ;
     HPS             hps ;
     RECTL           rcl ;

     switch (msg)
          {
          case WM_CREATE:
               hIcon = WinLoadPointer (HWND_DESKTOP, 0, ID_RESOURCE) ;
               hptr  = WinLoadPointer (HWND_DESKTOP, 0, IDP_CIRCLE) ;

               cxIcon = WinQuerySysValue (HWND_DESKTOP, SV_CXICON) ;
               cyIcon = WinQuerySysValue (HWND_DESKTOP, SV_CYICON) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_MOUSEMOVE:
               WinSetPointer (HWND_DESKTOP, hptr) ;
               return MRFROMSHORT (1) ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               WinQueryWindowRect (hwnd, &rcl) ;
               WinFillRect (hps, &rcl, CLR_CYAN) ;

               WinDrawPointer (hps, 0, 0, hIcon, DP_NORMAL) ;
               WinDrawPointer (hps, 0, cyClient - cyIcon, hIcon, DP_NORMAL) ;
               WinDrawPointer (hps, cxClient - cyIcon, 0, hIcon, DP_NORMAL) ;
               WinDrawPointer (hps, cxClient - cxIcon, cyClient - cyIcon,
                                    hIcon, DP_NORMAL) ;

               WinDrawPointer (hps, cxClient / 3, cyClient / 2, hIcon,
                                                       DP_HALFTONED) ;
               WinDrawPointer (hps, 2 * cxClient / 3, cyClient / 2, hIcon,
                                                       DP_INVERTED) ;
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               WinDestroyPointer (hIcon) ;
               WinDestroyPointer (hptr) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
