/*-----------------------------------------------------------
   ABOUT2.C -- Demonstration of About Box with New 3D Button
               (c) Charles Petzold, 1993
  -----------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include "about.h"

VOID APIENTRY RegisterNewBtnClass (HAB) ;         // In NEWBTN.C

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY AboutDlgProc  (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "About2" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU          | FCF_ICON ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     RegisterNewBtnClass (hab) ;

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
     switch (msg)
          {
          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_NEW:
                    case IDM_OPEN:
                    case IDM_SAVE:
                    case IDM_SAVEAS:
                         WinAlarm (HWND_DESKTOP, WA_NOTE) ;
                         return 0 ;

                    case IDM_ABOUT:
                         WinDlgBox (HWND_DESKTOP, hwnd, AboutDlgProc,
                                    NULLHANDLE, IDD_ABOUT, NULL) ;
                         return 0 ;
                    }
               break ;

          case WM_ERASEBACKGROUND:
               return MRFROMSHORT (1) ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

MRESULT EXPENTRY AboutDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     switch (msg)
          {
          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case DID_OK:
                    case DID_CANCEL:
                         WinDismissDlg (hwnd, TRUE) ;
                         return 0 ;
                    }
               break ;
          }
     return WinDefDlgProc (hwnd, msg, mp1, mp2) ;
     }
