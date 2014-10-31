/*--------------------------------------------------------
   COLORSCR.C -- Color Scroll using child window controls
                 (c) Charles Petzold, 1993
  --------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY ScrollProc (HWND, ULONG, MPARAM, MPARAM) ;

HWND  hwndScroll[3], hwndFocus ;
PFNWP pfnOldScroll[3] ;

int main (void)
     {
     static CHAR  szClientClass[] = "ColorScr" ;
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

     WinSetFocus (HWND_DESKTOP, hwndFocus = hwndScroll[0]) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CHAR  *apchColorLable[] = { "Red", "Green", "Blue" } ;
     static HWND  hwndLabel[3], hwndValue[3] ;
     static INT   cyChar, iColor[3] ;
     static LONG  alColorIndex[] = { CLR_RED, CLR_GREEN, CLR_BLUE } ;
     static RECTL rclRightHalf ;
     CHAR         szBuffer[10] ;
     FONTMETRICS  fm ;
     HPS          hps ;
     INT          i, id, cxClient, cyClient ;

     switch (msg)
          {
          case WM_CREATE :
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, (LONG) sizeof fm, &fm) ;
               cyChar = fm.lMaxBaselineExt ;
               WinReleasePS (hps) ;

               for (i = 0 ; i < 3 ; i++)
                    {
                    hwndScroll[i] = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_SCROLLBAR,       // Class
                                        NULL,               // Text
                                        WS_VISIBLE |        // Style
                                             SBS_VERT,
                                        0, 0,               // Position
                                        0, 0,               // Size
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        i,                  // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params

                    hwndLabel[i]  = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_STATIC,          // Class
                                        apchColorLable[i],  // Text
                                        WS_VISIBLE |        // Style
                                          SS_TEXT | DT_CENTER,
                                        0, 0,               // Position
                                        0, 0,               // Size
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        i + 3,              // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params

                    hwndValue[i]  = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_STATIC,          // Class
                                        "0",                // Text
                                        WS_VISIBLE |        // Style
                                          SS_TEXT | DT_CENTER,
                                        0, 0,               // Position
                                        0, 0,               // Size
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        i + 6,              // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params

                    pfnOldScroll[i] =
                              WinSubclassWindow (hwndScroll[i], ScrollProc) ;

                    WinSetPresParam (hwndScroll [i], PP_FOREGROUNDCOLORINDEX,
                                     sizeof (LONG), alColorIndex + i) ;

                    WinSendMsg (hwndScroll[i], SBM_SETSCROLLBAR,
				MPFROM2SHORT (0, 0), MPFROM2SHORT (0, 255)) ;
                    }
               return 0 ;

          case WM_SIZE :
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               for (i = 0 ; i < 3 ; i++)
                    {
                    WinSetWindowPos (hwndScroll[i], NULLHANDLE,
                                     (2 * i + 1) * cxClient / 14, 2 * cyChar,
                                     cxClient / 14, cyClient - 4 * cyChar,
                                     SWP_SIZE | SWP_MOVE) ;

                    WinSetWindowPos (hwndLabel[i], NULLHANDLE,
                                     (4 * i + 1) * cxClient / 28,
                                     cyClient - 3 * cyChar / 2,
                                     cxClient / 7, cyChar,
                                     SWP_SIZE | SWP_MOVE) ;

                    WinSetWindowPos (hwndValue[i], NULLHANDLE,
                                     (4 * i + 1) * cxClient / 28, cyChar / 2,
                                     cxClient / 7, cyChar,
                                     SWP_SIZE | SWP_MOVE) ;
                    }

               WinQueryWindowRect (hwnd, &rclRightHalf) ;
               rclRightHalf.xLeft = rclRightHalf.xRight / 2 ;
               return 0 ;

          case WM_VSCROLL :
               id = SHORT1FROMMP (mp1) ;          // ID of scroll bar

               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINEDOWN :
                         iColor[id] = min (255, iColor[id] + 1) ;
                         break ;

                    case SB_LINEUP :
                         iColor[id] = max (0, iColor[id] - 1) ;
                         break ;

                    case SB_PAGEDOWN :
                         iColor[id] = min (255, iColor[id] + 16) ;
                         break ;

                    case SB_PAGEUP :
                         iColor[id] = max (0, iColor[id] - 16) ;
                         break ;

                    case SB_SLIDERTRACK :
                         iColor[id] = SHORT1FROMMP (mp2) ;
                         break ;

                    default :
                         return 0 ;
                    }
               WinSendMsg (hwndScroll[id], SBM_SETPOS,
                           MPFROM2SHORT (iColor[id], 0), NULL) ;

               sprintf (szBuffer, "%d", iColor[id]) ;
               WinSetWindowText (hwndValue[id], szBuffer) ;
               WinInvalidateRect (hwnd, &rclRightHalf, FALSE) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               GpiCreateLogColorTable (hps, LCOL_RESET, LCOLF_RGB,
                                            0L, 0L, NULL) ;

               WinFillRect (hps, &rclRightHalf, (ULONG) iColor[0] << 16 |
                                                (ULONG) iColor[1] <<  8 |
                                                (ULONG) iColor[2]) ;
               WinEndPaint (hps) ;
               return 0 ;

          case WM_ERASEBACKGROUND:
               return MRFROMSHORT (1) ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

MRESULT EXPENTRY ScrollProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     INT id ;

     id = WinQueryWindowUShort (hwnd, QWS_ID) ;   // ID of scroll bar

     switch (msg)
          {
          case WM_CHAR:
               if (!(CHARMSG(&msg)->fs & KC_VIRTUALKEY))
                    break ;

               switch (CHARMSG(&msg)->vkey)
                    {
                    case VK_TAB:
                         if (!(CHARMSG(&msg)->fs & KC_KEYUP))
                              {
                              hwndFocus = hwndScroll[(id + 1) % 3] ;
                              WinSetFocus (HWND_DESKTOP, hwndFocus) ;
                              }
                         return MRFROMSHORT (1) ;

                    case VK_BACKTAB:
                         if (!(CHARMSG(&msg)->fs & KC_KEYUP))
                              {
                              hwndFocus = hwndScroll[(id + 2) % 3] ;
                              WinSetFocus (HWND_DESKTOP, hwndFocus) ;
                              }
                         return MRFROMSHORT (1) ;

                    default:
                         break ;
                    }
               break ;

          case WM_BUTTON1DOWN:
               WinSetFocus (HWND_DESKTOP, hwndFocus = hwnd) ;
               break ;
          }
     return pfnOldScroll[id] (hwnd, msg, mp1, mp2) ;
     }
