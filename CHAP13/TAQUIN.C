/*---------------------------------------
   TAQUIN.C -- Jeu de Taquin
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include "taquin.h"

#define NUMROWS        4      // greater than or equal to 2
#define NUMCOLS        4      // greater than or equal to 3
#define SCRAMBLEREP  100      // make larger if using more than 16 squares
#define SQUARESIZE    67      // in 1/100th inch

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "Taquin" ;
     static ULONG flFrameFlags = FCF_SYSMENU  | FCF_TITLEBAR  |
                                 FCF_BORDER   | FCF_MINBUTTON |
                                 FCF_MENU     | FCF_ICON      |
                                 FCF_TASKLIST ;
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
     static INT aiPuzzle[NUMROWS][NUMCOLS],
                iBlankRow, iBlankCol, cxSquare, cySquare ;
     CHAR       szNum[10] ;
     HPS        hps ;
     HWND       hwndFrame ;
     INT        iRow, iCol, iMouseRow, iMouseCol, i ;
     POINTL     ptl ;
     RECTL      rcl, rclInvalid, rclIntersect ;
     SIZEL      sizl ;

     switch (msg)
          {
          case WM_CREATE:
                              // Calculate square size in pixels

               hps = WinGetPS (hwnd) ;
               sizl.cx = sizl.cy = 0 ;
               GpiSetPS (hps, &sizl, PU_LOENGLISH) ;
               ptl.x = SQUARESIZE ;
               ptl.y = SQUARESIZE ;
               GpiConvert (hps, CVTC_PAGE, CVTC_DEVICE, 1L, &ptl) ;
               WinReleasePS (hps) ;

               cxSquare = ptl.x ;
               cySquare = ptl.y ;

                              // Calculate client window size and position

               rcl.xLeft   = (WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN) -
                                           NUMCOLS * cxSquare) / 2 ;
               rcl.yBottom = (WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN) -
                                           NUMROWS * cySquare) / 2 ;
               rcl.xRight  = rcl.xLeft   + NUMCOLS * cxSquare ;
               rcl.yTop    = rcl.yBottom + NUMROWS * cySquare ;

                              // Set frame window position and size

               hwndFrame = WinQueryWindow (hwnd, QW_PARENT) ;
               WinCalcFrameRect (hwndFrame, &rcl, FALSE) ;
               WinSetWindowPos  (hwndFrame, NULLHANDLE,
                                 rcl.xLeft, rcl.yBottom,
                                 rcl.xRight - rcl.xLeft,
                                 rcl.yTop - rcl.yBottom,
                                 SWP_MOVE | SWP_SIZE | SWP_ACTIVATE) ;

                              // Initialize the aiPuzzle array

               WinSendMsg (hwnd, WM_COMMAND, MPFROMSHORT (IDM_NORMAL), NULL) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, &rclInvalid) ;

                              // Draw the squares

               for (iRow = NUMROWS - 1 ; iRow >= 0 ; iRow--)
                    for (iCol = 0 ; iCol < NUMCOLS ; iCol++)
                         {
                         rcl.xLeft   = cxSquare * iCol ;
                         rcl.yBottom = cySquare * iRow ;
                         rcl.xRight  = rcl.xLeft   + cxSquare ;
                         rcl.yTop    = rcl.yBottom + cySquare ;

                         if (!WinIntersectRect (0, &rclIntersect,
                                                &rcl, &rclInvalid))
                              continue ;

                         if (iRow == iBlankRow && iCol == iBlankCol)
                              WinFillRect (hps, &rcl, CLR_BLACK) ;
                         else
                              {
                              WinDrawBorder (hps, &rcl, 5, 5,
                                             CLR_PALEGRAY, CLR_DARKGRAY,
                                             DB_STANDARD | DB_INTERIOR) ;

                              WinDrawBorder (hps, &rcl, 2, 2,
                                             CLR_BLACK, 0L, DB_STANDARD) ;

                              sprintf (szNum, "%d", aiPuzzle[iRow][iCol]) ;

                              WinDrawText (hps, -1, szNum,
                                           &rcl, CLR_WHITE, CLR_DARKGRAY,
                                           DT_CENTER | DT_VCENTER) ;
                              }
                         }
               WinEndPaint (hps) ;
               return 0 ;

          case WM_BUTTON1DOWN:
               iMouseCol = MOUSEMSG(&msg)->x / cxSquare ;
               iMouseRow = MOUSEMSG(&msg)->y / cySquare ;

                              // Check if mouse was in valid area

               if ( iMouseRow < 0          || iMouseCol < 0           ||
                    iMouseRow >= NUMROWS   || iMouseCol >= NUMCOLS    ||
                   (iMouseRow != iBlankRow && iMouseCol != iBlankCol) ||
                   (iMouseRow == iBlankRow && iMouseCol == iBlankCol))
                         break ;

                              // Move a row right or left

               if (iMouseRow == iBlankRow)
                    {
                    if (iMouseCol < iBlankCol)
                         for (iCol = iBlankCol ; iCol > iMouseCol ; iCol--)
                              aiPuzzle[iBlankRow][iCol] =
                                   aiPuzzle[iBlankRow][iCol - 1] ;
                    else
                         for (iCol = iBlankCol ; iCol < iMouseCol ; iCol++)
                              aiPuzzle[iBlankRow][iCol] =
                                   aiPuzzle[iBlankRow][iCol + 1] ;
                    }
                              // Move a column up or down
               else
                    {
                    if (iMouseRow < iBlankRow)
                         for (iRow = iBlankRow ; iRow > iMouseRow ; iRow--)
                              aiPuzzle[iRow][iBlankCol] =
                                   aiPuzzle[iRow - 1][iBlankCol] ;
                    else
                         for (iRow = iBlankRow ; iRow < iMouseRow ; iRow++)
                              aiPuzzle[iRow][iBlankCol] =
                                   aiPuzzle[iRow + 1][iBlankCol] ;
                    }
                              // Calculate invalid rectangle

               rcl.xLeft   = cxSquare *  min (iMouseCol, iBlankCol) ;
               rcl.yBottom = cySquare *  min (iMouseRow, iBlankRow) ;
               rcl.xRight  = cxSquare * (max (iMouseCol, iBlankCol) + 1) ;
               rcl.yTop    = cySquare * (max (iMouseRow, iBlankRow) + 1) ;

                              // Set new array and blank values

               iBlankRow = iMouseRow ;
               iBlankCol = iMouseCol ;
               aiPuzzle[iBlankRow][iBlankCol] = 0 ;

                              // Invalidate rectangle

               WinInvalidateRect (hwnd, &rcl, FALSE) ;
               break ;

          case WM_CHAR:
               if (!(CHARMSG(&msg)->fs & KC_VIRTUALKEY) ||
                     CHARMSG(&msg)->fs & KC_KEYUP)
                         return 0 ;

                              // Mimic a WM_BUTTON1DOWN message

               iMouseCol = iBlankCol ;
               iMouseRow = iBlankRow ;

               switch (CHARMSG(&msg)->vkey)
                    {
                    case VK_LEFT:   iMouseCol++ ;  break ;
                    case VK_RIGHT:  iMouseCol-- ;  break ;
                    case VK_UP:     iMouseRow-- ;  break ;
                    case VK_DOWN:   iMouseRow++ ;  break ;
                    default:        return 0 ;
                    }
               WinSendMsg (hwnd, WM_BUTTON1DOWN,
                           MPFROM2SHORT (iMouseCol * cxSquare,
                                         iMouseRow * cySquare), NULL) ;
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                              // Initialize aiPuzzle array

                    case IDM_NORMAL:
                    case IDM_INVERT:
                         for (iRow = 0 ; iRow < NUMROWS ; iRow++)
                              for (iCol = 0 ; iCol < NUMCOLS ; iCol++)
                                   aiPuzzle[iRow][iCol] = iCol + 1 +
                                        NUMCOLS * (NUMROWS - iRow - 1) ;

                         if (COMMANDMSG(&msg)->cmd == IDM_INVERT)
                              {
                              aiPuzzle[0][NUMCOLS-2] = NUMCOLS * NUMROWS - 2 ;
                              aiPuzzle[0][NUMCOLS-3] = NUMCOLS * NUMROWS - 1 ;
                              }
                         aiPuzzle[iBlankRow = 0][iBlankCol = NUMCOLS - 1] = 0 ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;

                              // Randomly scramble the squares

                    case IDM_SCRAMBLE:
                         WinSetPointer (HWND_DESKTOP, WinQuerySysPointer (
                                        HWND_DESKTOP, SPTR_WAIT, FALSE)) ;

                         srand ((int) WinGetCurrentTime (0)) ;

                         for (i = 0 ; i < SCRAMBLEREP ; i++)
                              {
                              WinSendMsg (hwnd, WM_BUTTON1DOWN,
                                   MPFROM2SHORT (rand() % NUMCOLS * cxSquare,
                                        iBlankRow * cySquare), NULL) ;
                              WinUpdateWindow (hwnd) ;

                              WinSendMsg (hwnd, WM_BUTTON1DOWN,
                                   MPFROM2SHORT (iBlankCol * cxSquare,
                                        rand() % NUMROWS * cySquare), NULL) ;
                              WinUpdateWindow (hwnd) ;
                              }
                         WinSetPointer (HWND_DESKTOP, WinQuerySysPointer (
                                        HWND_DESKTOP, SPTR_ARROW, FALSE));
                         return 0 ;
                    }
               break ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
