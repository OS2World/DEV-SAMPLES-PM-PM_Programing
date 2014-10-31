/*--------------------------------------------------------------
   BIGJOB3.C -- Peek Message approach to lengthy processing job
                (c) Charles Petzold, 1993
 ---------------------------------------------------------------*/

#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#include "bigjob.h"

int main (void)
     {
     return MainCode ("BigJob3", "BigJob3 - Message Peeking") ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static BOOL   fContinueCalc = FALSE ;
     static HAB    hab ;
     static HWND   hwndMenu ;
     static INT    iStatus = STATUS_READY ;
     static INT    iCurrentRep = IDM_10 ;
     static LONG   lCalcRep, lRepAmts [] = { 10, 100, 1000, 10000, 100000 } ;
     static ULONG  ulElapsedTime ;
     double        A ;
     LONG          lRep ;
     QMSG          qmsg ;

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

                         lCalcRep = lRepAmts [iCurrentRep - IDM_10] ;
                         fContinueCalc = TRUE ;
                         ulElapsedTime = WinGetCurrentTime (hab) ;

                         qmsg.msg = WM_NULL ;

                         for (A = 1.0, lRep = 0 ; lRep < lCalcRep ; lRep++)
                              {
                              A = Savage (A) ;

                              while (WinPeekMsg (hab, &qmsg, NULLHANDLE,
                                                 0, 0, PM_NOREMOVE))
                                   {
                                   if (qmsg.msg == WM_QUIT)
                                        break ;

                                   WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0) ;
                                   WinDispatchMsg (hab, &qmsg) ;

                                   if (!fContinueCalc)
                                        break ;
                                   }
                              if (!fContinueCalc || qmsg.msg == WM_QUIT)
                                   break ;
                              }
                         ulElapsedTime = WinGetCurrentTime (hab) - 
                                                  ulElapsedTime ;

                         if (!fContinueCalc || qmsg.msg == WM_QUIT)
                              iStatus = STATUS_READY ;
                         else
                              iStatus = STATUS_DONE ;

                         WinInvalidateRect (hwnd, NULL, FALSE) ;

                         WinEnableMenuItem (hwndMenu, IDM_START, TRUE) ;
                         WinEnableMenuItem (hwndMenu, IDM_ABORT, FALSE) ;
                         return 0 ;

                    case IDM_ABORT:
                         fContinueCalc = FALSE ;
                         return 0 ;
                    }
               break ;

          case WM_PAINT:
               PaintWindow (hwnd, iStatus, lCalcRep, ulElapsedTime) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
