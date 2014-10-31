/*--------------------------------------
   LIFE.C -- John Conway's Game of Life
             (c) Charles Petzold, 1993
  --------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "life.h"

#define ID_TIMER    1
#define GRID(x,y)   (pbGrid [(y) * xNumCells + (x)])

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

CHAR szClientClass [] = "Life" ;

int main (void)
     {
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

VOID ErrorMsg (HWND hwnd, CHAR *szMessage)
     {
     WinMessageBox (HWND_DESKTOP, hwnd, szMessage, szClientClass, 0,
                    MB_OK | MB_WARNING) ;
     }

VOID DrawCell (HPS hps, INT x, INT y, INT cxCell, INT cyCell, BYTE bCell)
     {
     RECTL rcl ;

     rcl.xLeft   = x * cxCell ;
     rcl.yBottom = y * cyCell ;
     rcl.xRight  = rcl.xLeft   + cxCell - 1 ;
     rcl.yTop    = rcl.yBottom + cyCell - 1 ;

     WinFillRect (hps, &rcl, (bCell & 1) ? CLR_NEUTRAL : CLR_BACKGROUND) ;
     }

VOID DoGeneration (HPS hps, PBYTE pbGrid, INT xNumCells, INT yNumCells,
                   INT cxCell, INT cyCell)
     {
     INT x, y, sSum ;

     for (y = 0 ; y < yNumCells - 1 ; y++)
          for (x = 0 ; x < xNumCells ; x++)
               {
               if (x == 0 || x == xNumCells - 1 || y == 0)
                    GRID (x,y) |= GRID (x,y) << 4 ;
               else
                    {
                    sSum = (GRID (x - 1, y    ) +           // Left
                            GRID (x - 1, y - 1) +           // Lower Left
                            GRID (x    , y - 1) +           // Lower
                            GRID (x + 1, y - 1)) >> 4 ;     // Lower Right

                    sSum += GRID (x + 1, y    ) +           // Right
                            GRID (x + 1, y + 1) +           // Upper Right
                            GRID (x    , y + 1) +           // Upper
                            GRID (x - 1, y + 1) ;           // Upper Left

                    sSum = (sSum | GRID (x, y)) & 0x0F ;

                    GRID (x, y) <<= 4 ;

                    if (sSum == 3)
                         GRID (x, y) |= 1 ;

                    if (GRID (x, y) != GRID (x, y) >> 4)
                         DrawCell (hps, x, y, cxCell, cyCell, GRID (x, y)) ;
                    }
               }
     }

VOID DisplayGenerationNum (HPS hps, INT xGen, INT yGen, INT iGeneration)
     {
     static CHAR szBuffer [24] = "Generation " ;
     POINTL      ptl ;

     ptl.x = xGen ;
     ptl.y = yGen ;

     sprintf (szBuffer + 11, "%d", iGeneration) ;

     GpiSavePS (hps) ;

     GpiSetBackMix (hps, BM_OVERPAINT) ;
     GpiCharStringAt (hps, &ptl, strlen (szBuffer), szBuffer) ;

     GpiRestorePS (hps, -1L) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static BOOL  fTimerGoing ;
     static HAB   hab ;
     static HWND  hwndMenu ;
     static INT   iGeneration, cxChar, cyChar, cyDesc, cxClient, cyClient,
                  xGenNum, yGenNum, cxCell, cyCell, xNumCells, yNumCells,
                  iScaleCell = 2 ;
     static PBYTE pbGrid ;
     FONTMETRICS  fm ;
     HPS          hps ;
     INT          x, y ;
     POINTL       ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hab = WinQueryAnchorBlock (hwnd) ;

               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;
               WinReleasePS (hps) ;

               hwndMenu = WinWindowFromID (
                               WinQueryWindow (hwnd, QW_PARENT),
                               FID_MENU) ;
               return 0 ;

          case WM_SIZE:
               if (pbGrid != NULL)
                    {
                    free (pbGrid) ;
                    pbGrid = NULL ;
                    }

               if (fTimerGoing)
                    {
                    WinStopTimer (hab, hwnd, ID_TIMER) ;
                    fTimerGoing = FALSE ;
                    }

               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               xGenNum = cxChar ;
               yGenNum = cyClient - cyChar + cyDesc ;

               cxCell = cxChar * 2 / iScaleCell ;
               cyCell = cyChar / iScaleCell ;

               xNumCells = cxClient / cxCell ;
               yNumCells = (cyClient - cyChar) / cyCell ;

               if (xNumCells <= 0 || yNumCells <= 0)
                    ErrorMsg (hwnd, "Not enough room for even one cell.") ;

               else if (NULL == (pbGrid = calloc (xNumCells, yNumCells)))
                    ErrorMsg (hwnd, "Not enough memory for this many cells.") ;

               WinEnableMenuItem (hwndMenu, IDM_SIZE,  TRUE) ;
               WinEnableMenuItem (hwndMenu, IDM_START, pbGrid != NULL) ;
               WinEnableMenuItem (hwndMenu, IDM_STOP,  FALSE) ;
               WinEnableMenuItem (hwndMenu, IDM_STEP,  pbGrid != NULL) ;
               WinEnableMenuItem (hwndMenu, IDM_CLEAR, pbGrid != NULL) ;

               iGeneration = 0 ;
               return 0 ;

          case WM_BUTTON1DOWN:
               x = MOUSEMSG(&msg)->x / cxCell ;
               y = MOUSEMSG(&msg)->y / cyCell ;

               if (pbGrid != NULL && !fTimerGoing && x < xNumCells &&
                                                     y < yNumCells)
                    {
                    hps = WinGetPS (hwnd) ;
                    DrawCell (hps, x, y, cxCell, cyCell, GRID (x, y) ^= 1) ;
                    WinReleasePS (hps) ;
                    }
               else
                    WinAlarm (HWND_DESKTOP, WA_WARNING) ;
               break ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_LARGE:
                    case IDM_SMALL:
                    case IDM_TINY:
                         WinCheckMenuItem (hwndMenu, iScaleCell, FALSE) ;
                         iScaleCell = COMMANDMSG(&msg)->cmd ;
                         WinCheckMenuItem (hwndMenu, iScaleCell, TRUE) ;

                         WinSendMsg (hwnd, WM_SIZE, NULL,
                                     MPFROM2SHORT (cxClient, cyClient)) ;

                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;

                    case IDM_START:
                         if (!WinStartTimer (hab, hwnd, ID_TIMER, 1))
                              ErrorMsg (hwnd, "Too many clocks or timers.") ;
                         else
                              {
                              fTimerGoing = TRUE ;

                              WinEnableMenuItem (hwndMenu, IDM_SIZE,  FALSE) ;
                              WinEnableMenuItem (hwndMenu, IDM_START, FALSE) ;
                              WinEnableMenuItem (hwndMenu, IDM_STOP,  TRUE) ;
                              WinEnableMenuItem (hwndMenu, IDM_STEP,  FALSE) ;
                              WinEnableMenuItem (hwndMenu, IDM_CLEAR, FALSE) ;
                              }
                         return 0 ;

                    case IDM_STOP:
                         WinStopTimer (hab, hwnd, ID_TIMER) ;
                         fTimerGoing = FALSE ;

                         WinEnableMenuItem (hwndMenu, IDM_SIZE,  TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_STOP,  FALSE) ;
                         WinEnableMenuItem (hwndMenu, IDM_STEP,  TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_CLEAR, TRUE) ;
                         return 0 ;

                    case IDM_STEP:
                         WinSendMsg (hwnd, WM_TIMER, NULL, NULL) ;
                         return 0 ;

                    case IDM_CLEAR:
                         iGeneration = 0L ;
                         memset (pbGrid, 0, xNumCells * yNumCells) ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;
                    }
               break ;

          case WM_TIMER:
               hps = WinGetPS (hwnd) ;

               DisplayGenerationNum (hps, xGenNum, yGenNum, ++iGeneration) ;
               DoGeneration (hps, pbGrid, xNumCells, yNumCells, cxCell, cyCell);

               WinReleasePS (hps) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               if (pbGrid != NULL)
                    {
                    for (x = 1 ; x <= xNumCells ; x++)
                         {
                         ptl.x = cxCell * x - 1 ;
                         ptl.y = 0 ;
                         GpiMove (hps, &ptl) ;

                         ptl.y = cyCell * yNumCells - 1 ;
                         GpiLine (hps, &ptl) ;
                         }

                    for (y = 1 ; y <= yNumCells ; y++)
                         {
                         ptl.x = 0 ;
                         ptl.y = cyCell * y - 1 ;
                         GpiMove (hps, &ptl) ;

                         ptl.x = cxCell * xNumCells - 1 ;
                         GpiLine (hps, &ptl) ;
                         }

                    for (y = 0 ; y < yNumCells ; y++)
                         for (x = 0 ; x < xNumCells ; x++)
                              if (GRID (x, y) & 1)
                                   DrawCell (hps, x, y, cxCell, cyCell,
                                             GRID (x, y)) ;

                    DisplayGenerationNum (hps, xGenNum, yGenNum, iGeneration) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               if (fTimerGoing)
                    WinStopTimer (hab, hwnd, ID_TIMER) ;

               if (pbGrid != NULL)
                    free (pbGrid) ;

               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
