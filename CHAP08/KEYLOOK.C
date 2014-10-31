/*----------------------------------------
   KEYLOOK.C -- Displays WM_CHAR Messages
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include "easyfont.h"

#define LCID_FIXEDFONT 1L
#define MAX_KEYS       100

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

CHAR szClientClass [] = "KeyLook" ;

int main (void)
     {
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
     if (hwndFrame != NULLHANDLE)
          {
          while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
               WinDispatchMsg (hab, &qmsg) ;

          WinDestroyWindow (hwndFrame) ;
          }
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CHAR   szHeader [] = "Scan  Rept  IN TG IC CM DK LK PD KU"
                                 " AL CT SH SC VK CH  Virt  Char" ;
     static CHAR   szUndrLn [] = "----  ----  -- -- -- -- -- -- -- --"
                                 " -- -- -- -- -- --  ----  ----" ;
     static CHAR   szFormat [] = "%4X %4dx  %2d %2d %2d %2d %2d %2d %2d %2d"
                                 " %2d %2d %2d %2d %2d %2d  %4X  %4X  %c" ;

     static INT    cxChar, cyChar, cyDesc, cxClient, cyClient, iNextKey ;
     static struct {
                   MPARAM mp1 ;
                   MPARAM mp2 ;
                   BOOL   fValid ;
                   }
                   key [MAX_KEYS] ;
     CHAR          szBuffer [80] ;
     FONTMETRICS   fm ;
     HPS           hps ;
     INT           iKey, iIndex, iFlag ;
     POINTL        ptl ;
     RECTL         rcl, rclInvalid ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               EzfQueryFonts (hps) ;

               if (!EzfCreateLogFont (hps, LCID_FIXEDFONT, FONTFACE_MONO,
                                                           FONTSIZE_10, 0))
                    {
                    WinReleasePS (hps) ;

                    WinMessageBox (HWND_DESKTOP, HWND_DESKTOP,
                         "Cannot find the System Monospaced font.",
                         szClientClass, 0, MB_OK | MB_WARNING) ;

                    return MRFROMSHORT (1) ;
                    }

               GpiSetCharSet (hps, LCID_FIXEDFONT) ;

               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;

               GpiSetCharSet (hps, LCID_DEFAULT) ;
               GpiDeleteSetId (hps, LCID_FIXEDFONT) ;
               WinReleasePS (hps) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_CHAR:
               key [iNextKey].mp1 = mp1 ;
               key [iNextKey].mp2 = mp2 ;
               key [iNextKey].fValid = TRUE ;

               iNextKey = (iNextKey + 1) % MAX_KEYS ;

               WinSetRect (hwnd, &rcl,
                           0, 2 * cyChar, cxClient, cyClient - 2 * cyChar) ;

               WinScrollWindow (hwnd, 0, cyChar, &rcl, &rcl, 0, NULL,
                                                 SW_INVALIDATERGN) ;
               WinUpdateWindow (hwnd) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, &rclInvalid) ;
               GpiErase (hps) ;
               EzfCreateLogFont (hps, LCID_FIXEDFONT, FONTFACE_MONO,
                                                      FONTSIZE_10, 0) ;
               GpiSetCharSet (hps, LCID_FIXEDFONT) ;

               ptl.x = cxChar ;
               ptl.y = cyDesc ;
               GpiCharStringAt (hps, &ptl, sizeof szHeader - 1L, szHeader) ;

               ptl.y += cyChar ;
               GpiCharStringAt (hps, &ptl, sizeof szUndrLn - 1L, szUndrLn) ;

               for (iKey = 0 ; iKey < MAX_KEYS ; iKey++)
                    {
                    ptl.y += cyChar ;

                    iIndex = (iNextKey - iKey - 1 + MAX_KEYS) % MAX_KEYS ;

                    if (ptl.y > rclInvalid.yTop ||
                              ptl.y > cyClient - 2 * cyChar ||
                                   !key [iIndex].fValid)
                         break ;
                         
                    mp1 = key [iIndex].mp1 ;
                    mp2 = key [iIndex].mp2 ;

                    iFlag = SHORT1FROMMP (mp1) ;

                    GpiCharStringAt (hps, &ptl, 
                         (LONG) sprintf (szBuffer, szFormat,
                                   CHAR4FROMMP (mp1),  // scan code
                                   CHAR3FROMMP (mp1),  // repeat count
                                   iFlag & KC_INVALIDCHAR ? 1 : 0,
                                   iFlag & KC_TOGGLE      ? 1 : 0,
                                   iFlag & KC_INVALIDCOMP ? 1 : 0,
                                   iFlag & KC_COMPOSITE   ? 1 : 0,
                                   iFlag & KC_DEADKEY     ? 1 : 0,
                                   iFlag & KC_LONEKEY     ? 1 : 0,
                                   iFlag & KC_PREVDOWN    ? 1 : 0,
                                   iFlag & KC_KEYUP       ? 1 : 0,
                                   iFlag & KC_ALT         ? 1 : 0,
                                   iFlag & KC_CTRL        ? 1 : 0,
                                   iFlag & KC_SHIFT       ? 1 : 0,
                                   iFlag & KC_SCANCODE    ? 1 : 0,
                                   iFlag & KC_VIRTUALKEY  ? 1 : 0,
                                   iFlag & KC_CHAR        ? 1 : 0,
                                   SHORT2FROMMP (mp2),  // virtual key
                                   SHORT1FROMMP (mp2),  // character
                                   iFlag & KC_CHAR ? SHORT1FROMMP (mp2) : ' '),
                              szBuffer) ;
                    }
               ptl.y = cyClient - cyChar + cyDesc ;
               GpiCharStringAt (hps, &ptl, sizeof szHeader - 1L, szHeader) ;

               ptl.y -= cyChar ;
               GpiCharStringAt (hps, &ptl, sizeof szUndrLn - 1L, szUndrLn) ;

               GpiSetCharSet (hps, LCID_DEFAULT) ;
               GpiDeleteSetId (hps, LCID_FIXEDFONT) ;
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
