/*--------------------------------------------
   SYSVALS.C -- System Values Display Program
                (c) Charles Petzold, 1993
  --------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysvals.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
LONG             RtJustCharStringAt (HPS, PPOINTL, LONG, PCHAR) ;

int main (void)
     {
     static CHAR  szClientClass [] = "SysVals" ;
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
     static HWND hwndHscroll, hwndVscroll ;
     static INT  iHscrollMax, iVscrollMax, iHscrollPos, iVscrollPos,
                 cxChar, cxCaps, cyChar, cyDesc, cxClient, cyClient,
                 cxTextTotal ;
     BOOL        fUpdate ;
     CHAR        szBuffer [10] ;
     FONTMETRICS fm ;
     HPS         hps ;
     INT         iLine, iPaintBeg, iPaintEnd, iHscrollInc, iVscrollInc ;
     POINTL      ptl ;
     RECTL       rclInvalid ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
     
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;

               cxChar = fm.lAveCharWidth ;
               cxCaps = (fm.fsType & 1 ? 2 : 3) * cxChar / 2 ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;

               WinReleasePS (hps) ;

               cxTextTotal = 32 * cxCaps + 38 * cxChar ;

               hwndHscroll = WinWindowFromID (
                                   WinQueryWindow (hwnd, QW_PARENT),
                                   FID_HORZSCROLL) ;

               hwndVscroll = WinWindowFromID (
                                   WinQueryWindow (hwnd, QW_PARENT),
                                   FID_VERTSCROLL) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               iHscrollMax = max (0, cxTextTotal - cxClient) ;
               iHscrollPos = min (iHscrollPos, iHscrollMax) ;

               WinSendMsg (hwndHscroll, SBM_SETSCROLLBAR,
                                        MPFROM2SHORT (iHscrollPos, 0),
                                        MPFROM2SHORT (0, iHscrollMax)) ;

               WinSendMsg (hwndHscroll, SBM_SETTHUMBSIZE,
                                        MPFROM2SHORT (cxClient, cxTextTotal),
                                        NULL) ;

               WinEnableWindow (hwndHscroll, iHscrollMax ? TRUE : FALSE) ;

               iVscrollMax = max (0, NUMLINES - cyClient / cyChar) ;
               iVscrollPos = min (iVscrollPos, iVscrollMax) ;

               WinSendMsg (hwndVscroll, SBM_SETSCROLLBAR,
                                        MPFROM2SHORT (iVscrollPos, 0),
                                        MPFROM2SHORT (0, iVscrollMax)) ;

               WinSendMsg (hwndVscroll, SBM_SETTHUMBSIZE,
                                        MPFROM2SHORT (cyClient / cyChar,
                                                      NUMLINES),
                                        NULL) ;

               WinEnableWindow (hwndVscroll, iVscrollMax ? TRUE : FALSE) ;
               return 0 ;

          case WM_HSCROLL:
               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINELEFT:
                         iHscrollInc = -cxCaps ;
                         break ;

                    case SB_LINERIGHT:
                         iHscrollInc = cxCaps ;
                         break ;

                    case SB_PAGELEFT:
                         iHscrollInc = -8 * cxCaps ;
                         break ;

                    case SB_PAGERIGHT:
                         iHscrollInc = 8 * cxCaps ;
                         break ;

                    case SB_SLIDERPOSITION:
                         iHscrollInc = SHORT1FROMMP (mp2) - iHscrollPos;
                         break ;

                    default:
                         iHscrollInc = 0 ;
                         break ;
                    }
               iHscrollInc = max (-iHscrollPos,
                             min (iHscrollInc, iHscrollMax - iHscrollPos)) ;

               if (iHscrollInc != 0)
                    {
                    iHscrollPos += iHscrollInc ;
                    WinScrollWindow (hwnd, -iHscrollInc, 0,
                                     NULL, NULL, NULLHANDLE, NULL,
                                     SW_INVALIDATERGN) ;

                    WinSendMsg (hwndHscroll, SBM_SETPOS,
                                MPFROMSHORT (iHscrollPos), NULL) ;
                    }
               return 0 ;

          case WM_VSCROLL:
               fUpdate = TRUE ;

               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINEUP:
                         iVscrollInc = -1 ;
                         break ;

                    case SB_LINEDOWN:
                         iVscrollInc = 1 ;
                         break ;

                    case SB_PAGEUP:
                         iVscrollInc = min (-1, -cyClient / cyChar) ;
                         break ;

                    case SB_PAGEDOWN:
                         iVscrollInc = max (1, cyClient / cyChar) ;
                         break ;

                    case SB_SLIDERTRACK:
                         fUpdate = FALSE ;
                         iVscrollInc = SHORT1FROMMP (mp2) - iVscrollPos;
                         break ;

                    case SB_SLIDERPOSITION:
                         iVscrollInc = SHORT1FROMMP (mp2) - iVscrollPos;
                         break ;

                    default:
                         fUpdate = FALSE ;
                         iVscrollInc = 0 ;
                         break ;
                    }

               iVscrollInc = max (-iVscrollPos,
                             min (iVscrollInc, iVscrollMax - iVscrollPos)) ;

               if (iVscrollInc != 0)
                    {
                    iVscrollPos += iVscrollInc ;
                    WinScrollWindow (hwnd, 0, cyChar * iVscrollInc,
                                     NULL, NULL, NULLHANDLE, NULL,
                                     SW_INVALIDATERGN) ;

                    WinUpdateWindow (hwnd) ;
                    }

               if (fUpdate)
                    WinSendMsg (hwndVscroll, SBM_SETPOS,
                                MPFROMSHORT (iVscrollPos), NULL) ;
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
               hps = WinBeginPaint (hwnd, NULLHANDLE, &rclInvalid) ;
               GpiErase (hps) ;

               iPaintBeg = max (0, iVscrollPos +
                              (cyClient - rclInvalid.yTop) / cyChar) ;
               iPaintEnd = min (NUMLINES, iVscrollPos +
                              (cyClient - rclInvalid.yBottom)
                                   / cyChar + 1) ;

               for (iLine = iPaintBeg ; iLine < iPaintEnd ; iLine++)
                    {
                    ptl.x = cxCaps - iHscrollPos ;
                    ptl.y = cyClient - cyChar * (iLine + 1 - iVscrollPos)
                                     + cyDesc ;

                    GpiCharStringAt (hps, &ptl,
                              strlen (sysvals[iLine].szIdentifier),
                              sysvals[iLine].szIdentifier) ;

                    ptl.x += 24 * cxCaps ;
                    GpiCharStringAt (hps, &ptl,
                              strlen (sysvals[iLine].szDescription),
                              sysvals[iLine].szDescription) ;

                    sprintf (szBuffer, "%d",
                             WinQuerySysValue (HWND_DESKTOP,
                                               sysvals[iLine].sIndex)) ;

                    ptl.x += 38 * cxChar + 6 * cxCaps ;
                    RtJustCharStringAt (hps, &ptl, strlen (szBuffer),
                                        szBuffer) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

LONG RtJustCharStringAt (HPS hps, PPOINTL pptl, LONG lLength, PCHAR pchText)
     {
     POINTL aptlTextBox[TXTBOX_COUNT] ;

     GpiQueryTextBox (hps, lLength, pchText, TXTBOX_COUNT, aptlTextBox) ;

     pptl->x -= aptlTextBox[TXTBOX_CONCAT].x ;

     return GpiCharStringAt (hps, pptl, lLength, pchText) ;
     }
