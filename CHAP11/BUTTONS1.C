/*-----------------------------------------
   BUTTONS1.C -- Push Button Demonstration
                 (c) Charles Petzold, 1993
  -----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "Buttons1" ;
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
     static CHAR  *szButtonLabel[] = { "Smaller", "Larger" } ;
     static HWND  hwndFrame, hwndButton[2] ;
     static INT   cxClient, cyClient, cxChar, cyChar ;
     FONTMETRICS  fm ;
     HPS          hps ;
     INT          id ;
     RECTL        rcl ;

     switch (msg)
          {
          case WM_CREATE :
               hwndFrame = WinQueryWindow (hwnd, QW_PARENT) ;

               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyChar = fm.lMaxBaselineExt ;
               WinReleasePS (hps) ;

               for (id = 0 ; id < 2 ; id++)
                    hwndButton[id] = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_BUTTON,          // Class
                                        szButtonLabel[id],  // Text
                                        WS_VISIBLE |        // Style
                                             BS_PUSHBUTTON,
                                        0, 0,               // Position
                                        12 * cxChar,        // Width
                                        2 * cyChar,         // Height
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        id,                 // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params
               return 0 ;

          case WM_SIZE :
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               for (id = 0 ; id < 2 ; id++)
                    WinSetWindowPos (hwndButton[id], NULLHANDLE,
                              cxClient / 2 + (14 * id - 13) * cxChar,
                              (cyClient - 2 * cyChar) / 2,
                              0, 0, SWP_MOVE) ;
               return 0 ;

          case WM_COMMAND:
               WinQueryWindowRect (hwnd, &rcl) ;
               WinMapWindowPoints (hwnd, HWND_DESKTOP, (PPOINTL) &rcl, 2) ;

               switch (COMMANDMSG(&msg)->cmd)               // Child ID
                    {
                    case 0:                                 // "Smaller"
                         rcl.xLeft   += cxClient / 20 ;
                         rcl.xRight  -= cxClient / 20 ;
                         rcl.yBottom += cyClient / 20 ;
                         rcl.yTop    -= cyClient / 20 ;
                         break ;

                    case 1:                                 // "Larger"
                         rcl.xLeft   -= cxClient / 20 ;
                         rcl.xRight  += cxClient / 20 ;
                         rcl.yBottom -= cyClient / 20 ;
                         rcl.yTop    += cyClient / 20 ;
                         break ;
                    }
               WinCalcFrameRect (hwndFrame, &rcl, FALSE) ;

               WinSetWindowPos (hwndFrame, NULLHANDLE,
                                rcl.xLeft, rcl.yBottom,
                                rcl.xRight - rcl.xLeft,
                                rcl.yTop   - rcl.yBottom,
				SWP_MOVE | SWP_SIZE) ;
               return 0 ;

          case WM_ERASEBACKGROUND:
               return MRFROMSHORT (1) ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
