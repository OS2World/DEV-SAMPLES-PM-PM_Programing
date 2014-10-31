/*---------------------------------------------------------------
   BIGJOB4.C -- Second thread approach to lengthy processing job
                (c) Charles Petzold, 1993
 ----------------------------------------------------------------*/

#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#include <process.h>
#include <stdlib.h>
#include "bigjob.h"

VOID CalcThread (PVOID) ;

int main (void)
     {
     return MainCode ("BigJob4", "BigJob4 - A Second Thread") ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CALCPARAM cp ;
     static HWND      hwndMenu ;
     static int       tidCalc ;
     static INT       iCurrentRep = IDM_10 ;
     static INT       iStatus = STATUS_READY ;
     static LONG      lRepAmts [] = { 10, 100, 1000, 10000, 100000 } ;
     static ULONG     ulElapsedTime ;

     switch (msg)
          {
          case WM_CREATE:
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
                         cp.hwnd = hwnd ;
                         cp.lCalcRep = lRepAmts [iCurrentRep - IDM_10] ;
                         cp.fContinueCalc = TRUE ;

                         if (-1 == (tidCalc = _beginthread (CalcThread,
#ifdef __IBMC__
                                                            NULL,
#endif
                                                            STACKSIZE, &cp)))
                              {
                              WinAlarm (HWND_DESKTOP, WA_ERROR) ;
                              return 0 ;
                              }

                         iStatus = STATUS_WORKING ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         WinEnableMenuItem (hwndMenu, IDM_START, FALSE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, TRUE) ;
                         return 0 ;

                    case IDM_ABORT:
                         cp.fContinueCalc = FALSE ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
                         return 0 ;
                    }
               break ;

          case WM_CALC_DONE:
               iStatus = STATUS_DONE ;
               ulElapsedTime = LONGFROMMP (mp1) ;
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
               WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
               return 0 ;

          case WM_CALC_ABORTED:
               iStatus = STATUS_READY ;
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
               return 0 ;

          case WM_PAINT:
               PaintWindow (hwnd, iStatus, cp.lCalcRep, ulElapsedTime) ;
               return 0 ;

          case WM_DESTROY:
               if (iStatus == STATUS_WORKING)
                    DosKillThread (tidCalc) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

VOID CalcThread (PVOID pArg)
     {
     double     A ;
     HAB        hab ;
     LONG       lRep, lTime ;
     PCALCPARAM pcp ;

     hab = WinInitialize (0) ;
     pcp = (PCALCPARAM) pArg ;
     lTime = WinGetCurrentTime (hab) ;

     for (A = 1.0, lRep = 0 ; lRep < pcp->lCalcRep &&
                              pcp->fContinueCalc ; lRep++)
          A = Savage (A) ;

     DosEnterCritSec () ;     // So thread is dead when message retrieved

     if (pcp->fContinueCalc)
          {
          lTime = WinGetCurrentTime (hab) - lTime ;
          WinPostMsg (pcp->hwnd, WM_CALC_DONE, MPFROMLONG (lTime), NULL) ;
          }
     else
          WinPostMsg (pcp->hwnd, WM_CALC_ABORTED, NULL, NULL) ;

     WinTerminate (hab) ;
     _endthread () ;
     }
