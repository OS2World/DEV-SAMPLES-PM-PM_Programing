/*-------------------------------------------------------
   BIGJOB1.C -- Naive approach to lengthy processing job
                (c) Charles Petzold, 1993
 --------------------------------------------------------*/

#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#include "bigjob.h"

int main (void)
     {
     return MainCode ("BigJob1", "BigJob1 - The Bad Program") ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static HAB   hab ;
     static HWND  hwndMenu ;
     static INT   iCurrentRep = IDM_10 ;
     static INT   iStatus = STATUS_READY ;
     static LONG  lCalcRep, lRepAmts [] = { 10, 100, 1000, 10000, 100000 } ;
     static ULONG ulElapsedTime ;
     double       A ;
     LONG         lRep ;

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
                         WinEnableMenuItem (hwndMenu, IDM_START, FALSE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, TRUE) ;

                         iStatus = STATUS_WORKING ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         WinUpdateWindow (hwnd) ;

                         WinSetPointer (HWND_DESKTOP,
                                   WinQuerySysPointer (HWND_DESKTOP,
                                                       SPTR_WAIT, FALSE)) ;

                         if (WinQuerySysValue (HWND_DESKTOP, SV_MOUSEPRESENT)
                                   == 0)
                              WinShowPointer (HWND_DESKTOP, TRUE) ;

                         lCalcRep = lRepAmts [iCurrentRep - IDM_10] ;
                         ulElapsedTime = WinGetCurrentTime (hab) ;

                         for (A = 1.0, lRep = 0 ; lRep < lCalcRep ; lRep++)
                              A = Savage (A) ;

                         ulElapsedTime = WinGetCurrentTime (hab) -
                                        ulElapsedTime ;

                         if (WinQuerySysValue (HWND_DESKTOP, SV_MOUSEPRESENT)
                                   == 0)
                              WinShowPointer (HWND_DESKTOP, FALSE) ;

                         WinSetPointer (HWND_DESKTOP,
                                   WinQuerySysPointer (HWND_DESKTOP,
                                                       SPTR_ARROW, FALSE)) ;
                         iStatus = STATUS_DONE ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         WinUpdateWindow (hwnd) ;

                         WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
                         return 0 ;

                    case IDM_ABORT:     // Not much we can do here
                         return 0 ;
                    }
               break ;

          case WM_PAINT:
               PaintWindow (hwnd, iStatus, lCalcRep, ulElapsedTime) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
