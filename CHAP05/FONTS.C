/*--------------------------------------
   FONTS.C -- GPI Image Fonts
              (c) Charles Petzold, 1993
  --------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include "easyfont.h"

#define LCID_MYFONT 1L

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Fonts" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_VERTSCROLL    | FCF_HORZSCROLL ;
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
     static CHAR *szFace[] = { "System", "Monospaced", "Courier",
                               "Helv",   "Tms Rmn" } ;
     static CHAR *szSize[] = { "8", "10", "12", "14", "18", "24" } ;
     static CHAR *szSel[]  = { "Normal",  "Italic",  "Underscore",
                               "Strike-out", "Bold" } ;
     static CHAR szBuffer[80] ;
     static HWND hwndVscroll, hwndHscroll ;
     static INT  idFace[] = { FONTFACE_SYSTEM, FONTFACE_MONO,
                              FONTFACE_COUR,   FONTFACE_HELV,
                              FONTFACE_TIMES } ;
     static INT  idSize[] = { FONTSIZE_8,  FONTSIZE_10, FONTSIZE_12,
                              FONTSIZE_14, FONTSIZE_18, FONTSIZE_24 } ;
     static INT  afiSel[] = { 0, FATTR_SEL_ITALIC,    FATTR_SEL_UNDERSCORE,
                                 FATTR_SEL_STRIKEOUT, FATTR_SEL_BOLD } ;
     static INT  iVscrollMax = sizeof idFace / sizeof idFace[0] - 1,
                 iHscrollMax = sizeof afiSel / sizeof afiSel[0] - 1,
                 cyClient, iHscrollPos, iVscrollPos ;
     FONTMETRICS fm ;
     HPS         hps;
     HWND        hwndFrame ;
     INT         iSize ;
     POINTL      ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               EzfQueryFonts (hps) ;
               WinReleasePS (hps) ;

               hwndFrame   = WinQueryWindow (hwnd, QW_PARENT) ;
               hwndVscroll = WinWindowFromID (hwndFrame, FID_VERTSCROLL) ;
               hwndHscroll = WinWindowFromID (hwndFrame, FID_HORZSCROLL) ;

               WinSendMsg (hwndVscroll, SBM_SETSCROLLBAR,
                           MPFROM2SHORT (iVscrollPos, 0),
                           MPFROM2SHORT (0, iVscrollMax)) ;

               WinSendMsg (hwndHscroll, SBM_SETSCROLLBAR,
                           MPFROM2SHORT (iHscrollPos, 0),
                           MPFROM2SHORT (0, iHscrollMax)) ;
               return 0 ;

          case WM_SIZE:
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_VSCROLL:
               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINEUP:
                    case SB_PAGEUP:
                         iVscrollPos = max (0, iVscrollPos - 1) ;
                         break ;

                    case SB_LINEDOWN:
                    case SB_PAGEDOWN:
                         iVscrollPos = min (iVscrollMax, iVscrollPos + 1) ;
                         break ;

                    case SB_SLIDERPOSITION:
                         iVscrollPos = SHORT1FROMMP (mp2) ;
                         break ;

                    default:
                         return 0 ;
                    }
               WinSendMsg (hwndVscroll, SBM_SETPOS,
                           MPFROM2SHORT (iVscrollPos, 0), NULL) ;

               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_HSCROLL:
               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINELEFT:
                    case SB_PAGELEFT:
                         iHscrollPos = max (0, iHscrollPos - 1) ;
                         break ;

                    case SB_LINERIGHT:
                    case SB_PAGERIGHT:
                         iHscrollPos = min (iHscrollMax, iHscrollPos + 1) ;
                         break ;

                    case SB_SLIDERPOSITION:
                         iHscrollPos = SHORT1FROMMP (mp2) ;
                         break ;

                    default:
                         return 0 ;
                    }
               WinSendMsg (hwndHscroll, SBM_SETPOS,
                           MPFROM2SHORT (iHscrollPos, 0), NULL) ;

               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_CHAR:
               switch (CHARMSG(&msg)->vkey)
                    {
                    case VK_LEFT:
                    case VK_RIGHT:
                         return WinSendMsg (hwndHscroll, msg, mp1, mp2) ;
                    case VK_UP:
                    case VK_DOWN:
                    case VK_PAGEUP:
                    case VK_PAGEDOWN:
                         return WinSendMsg (hwndVscroll, msg, mp1, mp2) ;
                    }
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               ptl.x = 0 ;
               ptl.y = cyClient ;

               for (iSize = 0 ; iSize < 6 ; iSize++)
                    if (EzfCreateLogFont (hps, LCID_MYFONT,
                                          idFace [iVscrollPos],
                                          idSize [iSize],
                                          afiSel [iHscrollPos]))
                         {
                         GpiSetCharSet (hps, LCID_MYFONT) ;
                         GpiQueryFontMetrics (hps, sizeof fm, &fm) ;

                         ptl.y -= fm.lMaxBaselineExt ;

                         GpiCharStringAt (hps, &ptl,
                              sprintf (szBuffer, "%s, %s point, %s",
                                       szFace [iVscrollPos],
                                       szSize [iSize],
                                       szSel  [iHscrollPos]),
                              szBuffer) ;

                         GpiSetCharSet (hps, LCID_DEFAULT) ;
                         GpiDeleteSetId (hps, LCID_MYFONT) ;
                         }

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
