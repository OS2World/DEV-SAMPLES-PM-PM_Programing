/*--------------------------------------------------
   PATTDLG.C -- Select GPI Patterns from Dialog Box
                (c) Charles Petzold, 1993
  --------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include "pattdlg.h"

typedef struct
     {
     INT  iPattern ;
     INT  iColor ;
     BOOL fBorder ;
     }
     PATTERNSDATA ;

typedef PATTERNSDATA *PPATTERNSDATA ;

MRESULT EXPENTRY ClientWndProc  (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY AboutDlgProc   (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY PatternDlgProc (HWND, ULONG, MPARAM, MPARAM) ;

#define WM_USER_QUERYSAVE (WM_USER + 1)

HAB  hab ;

int main (void)
     {
     static CHAR  szClientClass[] = "PattDlg" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU ;
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

     WinSendMsg (hwndClient, WM_USER_QUERYSAVE, NULL, NULL) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CHAR         szAppName [] = "PATTDLG" ;
     static CHAR         szKeyName [] = "SETTINGS" ;
     static INT          cxClient, cyClient ;
     static PATTERNSDATA pdCurrent = { IDD_DENSE1, IDD_BKGRND, TRUE } ;
     HPS                 hps ;
     POINTL              ptl ;
     ULONG               ulDataLength ;

     switch (msg)
          {
          case WM_CREATE:
               ulDataLength = sizeof pdCurrent ;

               PrfQueryProfileData (HINI_USERPROFILE, szAppName, szKeyName,
                                    &pdCurrent, & ulDataLength) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_PATTERNS:
                         if (WinDlgBox (HWND_DESKTOP, hwnd, PatternDlgProc,
                                        NULLHANDLE, IDD_PATTERNS, &pdCurrent))

                              WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;

                    case IDM_ABOUT:
                         WinDlgBox (HWND_DESKTOP, hwnd, AboutDlgProc,
                                    NULLHANDLE, IDD_ABOUT, NULL) ;
                         return 0 ;
                    }
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               GpiSetColor (hps, pdCurrent.iColor -
                                 IDD_BKGRND + CLR_BACKGROUND) ;

               GpiSetPattern (hps, pdCurrent.iPattern -
                                   IDD_DENSE1 + PATSYM_DENSE1) ;

               ptl.x = cxClient / 4 ;
               ptl.y = cyClient / 4 ;
               GpiMove (hps, &ptl) ;

               ptl.x *= 3 ;
               ptl.y *= 3 ;
               GpiBox (hps, pdCurrent.fBorder ? DRO_OUTLINEFILL : DRO_FILL,
                            &ptl, 0L, 0L) ;

               WinEndPaint (hps) ;
               return 0 ;

          case WM_USER_QUERYSAVE:
               if (MBID_YES == WinMessageBox (HWND_DESKTOP, hwnd,
                                    "Save current settings?", szAppName, 0,
                                    MB_YESNO | MB_QUERY))

                    PrfWriteProfileData (HINI_USERPROFILE, szAppName, szKeyName,
                                         &pdCurrent, sizeof pdCurrent) ;
               return 0 ;
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

MRESULT EXPENTRY PatternDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static PATTERNSDATA  pdLocal ;
     static PPATTERNSDATA ppdCurrent ;

     switch (msg)
          {
          case WM_INITDLG:
               ppdCurrent = PVOIDFROMMP (mp2) ;
               pdLocal = *ppdCurrent ;

               WinCheckButton (hwnd, pdLocal.iPattern, TRUE) ;
               WinCheckButton (hwnd, pdLocal.iColor,   TRUE) ;
               WinCheckButton (hwnd, IDD_BORDER,       pdLocal.fBorder) ;

               WinSetFocus (HWND_DESKTOP,
                            WinWindowFromID (hwnd, pdLocal.iPattern)) ;

               return MRFROMSHORT (1) ;

          case WM_CONTROL:
               if (SHORT1FROMMP (mp1) >= IDD_DENSE1 &&
                   SHORT1FROMMP (mp1) <= IDD_DIAGHATCH)
                    {
                    WinCheckButton (hwnd, pdLocal.iPattern, FALSE) ;
                    pdLocal.iPattern = SHORT1FROMMP (mp1) ;
                    WinCheckButton (hwnd, pdLocal.iPattern, TRUE) ;
                    }

               else if (SHORT1FROMMP (mp1) >= IDD_BKGRND &&
                        SHORT1FROMMP (mp1) <= IDD_PALEGRAY)
                    {
                    WinCheckButton (hwnd, pdLocal.iColor, FALSE) ;
                    pdLocal.iColor = SHORT1FROMMP (mp1) ;
                    WinCheckButton (hwnd, pdLocal.iColor, TRUE) ;
                    }
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case DID_OK:
                         pdLocal.fBorder = WinQueryButtonCheckstate
                                                     (hwnd, IDD_BORDER) ;
                         *ppdCurrent = pdLocal ;

                         WinDismissDlg (hwnd, TRUE) ;
                         return 0 ;

                    case DID_CANCEL:
                         WinDismissDlg (hwnd, FALSE) ;
                         return 0 ;
                    }
               break ;
          }
     return WinDefDlgProc (hwnd, msg, mp1, mp2) ;
     }
