/*----------------------------------------
   POPMENU.C -- Popup Menu Demonstration
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include "popmenu.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY AboutDlgProc  (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "PopMenu" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, 0L, 0) ;

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
     static HWND hwndMenuPopup ;
     HPS         hps ;
     POINTL      ptlMouse ;

     switch (msg)
          {
          case WM_CREATE:
               hwndMenuPopup = WinLoadMenu (hwnd, 0, ID_RESOURCE) ;
               return 0 ;

          case WM_BUTTON2DOWN:
               ptlMouse.x = SHORT1FROMMP (mp1) ;
               ptlMouse.y = SHORT2FROMMP (mp1) ;

               WinMapWindowPoints (hwnd, HWND_DESKTOP, &ptlMouse, 1) ;

               WinPopupMenu (HWND_DESKTOP, hwnd, hwndMenuPopup,
                             ptlMouse.x, ptlMouse.y, 0,
                             PU_HCONSTRAIN   | PU_VCONSTRAIN   |
                             PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 |
                             PU_KEYBOARD) ;
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_ABOUT:
                         WinMessageBox (HWND_DESKTOP, hwnd,
                                        "PopMenu (c) Charles Petzold, 1993",
                                        "PopMenu", 0, MB_OK) ;
                         return 0 ;
                    }
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
