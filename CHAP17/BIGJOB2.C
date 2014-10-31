/*-------------------------------------------------------
   BIGJOB2.C -- Timer approach to lengthy processing job
                (c) Charles Petzold, 1993
 --------------------------------------------------------*/

#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#include "bigjob.h"

#define ID_TIMER 1

int main (void)
     {
     return MainCode ("BigJob2", "BigJob2 - The Timer") ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static double A ;
     static HAB    hab ;
     static HWND   hwndMenu ;
     static INT    iCurrentRep = IDM_10 ;
     static INT    iStatus = STATUS_READY ;
     static LONG   lRep, lCalcRep,
                   lRepAmts [] = { 10, 100, 1000, 10000, 100000 } ;
     static ULONG  ulElapsedTime ;

     switch (msg)
          {
          case WM_CREATE:
               hab = WinQueryAnchorBlock (hwnd) ;

               hwndMenu = WinWindowFromID (
                               WinQueryWindow (hwnd, QW_PARENT),
                               FID_MENU) ;
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_10:
                    case IDM_100:
                    case IDM_1000:
                    case IDM_10000:
                    case IDM_100000:
                         WinCheckMenuItem (hwndMenu, iCurrentRep, FALSE) ;
                         iCurrentRep = COMMANDMSG(&msg)->cmd ;
                         WinCheckMenuItem (hwndMenu, iCurrentRep, TRUE) ;
                         return 0 ;

                    case IDM_START:
                         if (!WinStartTimer (hab, hwnd, ID_TIMER, 0))
                              {
                              WinAlarm (HWND_DESKTOP, WA_ERROR) ;
                              return 0 ;
                              }
                         WinEnableMenuItem (hwndMenu, IDM_START, FALSE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, TRUE) ;

                         iStatus = STATUS_WORKING ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;

                         lCalcRep = lRepAmts [iCurrentRep - IDM_10] ;
                         ulElapsedTime = WinGetCurrentTime (hab) ;
                         A = 1.0 ;
                         lRep = 0 ;

                         return 0 ;

                    case IDM_ABORT:
                         WinStopTimer (hab, hwnd, ID_TIMER) ;

                         iStatus = STATUS_READY ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;

                         WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
                         return 0 ;
                    }
               break ;

          case WM_TIMER:
               A = Savage (A) ;

               if (++lRep == lCalcRep)
                    {
                    ulElapsedTime = WinGetCurrentTime (hab) -
                                        ulElapsedTime ;

                    WinStopTimer (hab, hwnd, ID_TIMER) ;

                    iStatus = STATUS_DONE ;
                    WinInvalidateRect (hwnd, NULL, FALSE) ;

                    WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
                    WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
                    }
               return 0 ;

          case WM_PAINT:
               PaintWindow (hwnd, iStatus, lCalcRep, ulElapsedTime) ;
               return 0 ;

          case WM_DESTROY:
               if (iStatus == STATUS_WORKING)
                    WinStopTimer (hab, hwnd, ID_TIMER) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
