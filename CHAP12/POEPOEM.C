/*--------------------------------------------------------
   POEPOEM.C -- Demonstrates Programmer-Defined Resources
                (c) Charles Petzold, 1993
  --------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#include <os2.h>
#include <stdlib.h>
#include "poepoem.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [10] ;
     static CHAR  szTitleBar [64] ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_VERTSCROLL    | FCF_ICON ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinLoadString (hab, 0, IDS_CLASS, sizeof szClientClass, szClientClass) ;
     WinLoadString (hab, 0, IDS_TITLE, sizeof szTitleBar,    szTitleBar) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, szTitleBar,
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
     static CHAR *pResource ;
     static HWND hwndScroll ;
     static INT  cyClient, cxChar, cyChar, cyDesc,
                 iScrollPos, iNumLines ;
     FONTMETRICS fm ;
     HPS         hps ;
     INT         iLineLength, iLine ;
     PCHAR       pText ;
     POINTL      ptl ;
     ULONG       ulMemSize, ulMemFlags ;
     
     switch (msg)
          {
          case WM_CREATE:

                    /*-----------------------------------------
                       Load the resource, get size and address
                      -----------------------------------------*/

               DosGetResource (0, IDT_TEXT, IDT_POEM, (PPVOID) & pResource) ;
               DosQueryMem (pResource, &ulMemSize, &ulMemFlags) ;

                    /*-----------------------------------------------
                       Determine how many text lines are in resource
                      -----------------------------------------------*/

               pText = pResource ;

               while (pText - pResource < ulMemSize)
                    {
                    if (*pText == '\0' || *pText == '\x1A')
                         break ;

                    if (*pText == '\r')
                         iNumLines ++ ;

                    pText++ ;
                    }

                    /*------------------------------------------
                       Initialize scroll bar range and position
                      ------------------------------------------*/

               hwndScroll = WinWindowFromID (
                                   WinQueryWindow (hwnd, QW_PARENT),
                                   FID_VERTSCROLL) ;

               WinSendMsg (hwndScroll, SBM_SETSCROLLBAR,
                                       MPFROM2SHORT (iScrollPos, 0),
                                       MPFROM2SHORT (0, iNumLines - 1)) ;

                    /*----------------------
                       Query character size
                      ----------------------*/

               hps = WinGetPS (hwnd) ;

               GpiQueryFontMetrics (hps, (LONG) sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;

               WinReleasePS (hps) ;
               return 0 ;

          case WM_SIZE:
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_CHAR:
               return WinSendMsg (hwndScroll, msg, mp1, mp2) ;

          case WM_VSCROLL:
               switch (SHORT2FROMMP (mp2))
                    {
                    case SB_LINEUP:
                         iScrollPos -= 1 ;
                         break ;

                    case SB_LINEDOWN:
                         iScrollPos += 1 ;
                         break ;

                    case SB_PAGEUP:
                         iScrollPos -= cyClient / cyChar ;
                         break ;

                    case SB_PAGEDOWN:
                         iScrollPos += cyClient / cyChar ;
                         break ;

                    case SB_SLIDERPOSITION:
                         iScrollPos = SHORT1FROMMP (mp2) ;
                         break ;

                    default:
                         return 0 ;
                    }
               iScrollPos = max (0, min (iScrollPos, iNumLines - 1)) ;

               WinSendMsg (hwndScroll, SBM_SETPOS,
                           MPFROM2SHORT (iScrollPos, 0), NULL) ;

               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               pText = pResource ;

               for (iLine = 0 ; iLine < iNumLines ; iLine++)
                    {
                    iLineLength = 0 ;

                    while (pText [iLineLength] != '\r')
                         iLineLength ++ ;

                    ptl.x = cxChar ;
                    ptl.y = cyClient - cyChar * (iLine + 1 - iScrollPos)
                                     + cyDesc ;
                    
                    GpiCharStringAt (hps, &ptl, (LONG) iLineLength, pText) ;

                    pText += iLineLength + 2 ;
                    }
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               DosFreeResource (pResource) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
