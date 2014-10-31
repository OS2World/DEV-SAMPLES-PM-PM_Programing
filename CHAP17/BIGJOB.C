/*-------------------------------------------------------
   BIGJOB.C -- Common functions used in BIGJOBx Programs
               (c) Charles Petzold, 1993
  -------------------------------------------------------*/

#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#include <math.h>
#include <stdio.h>
#include "bigjob.h"

int MainCode (CHAR *szClientClass, CHAR *szTitleBarText)
     {
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass,
                                     szTitleBarText,
                                     0L, 0, ID_RESOURCE, &hwndClient) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

double Savage (double A)
     {
     return tan (atan (exp (log (sqrt (A * A))))) + 1.0 ;
     }

VOID PaintWindow (HWND hwnd, SHORT sStatus, LONG lCalcRep, ULONG ulTime)
     {
     static CHAR *szMessage [3] = { "Ready", "Working...",
                                    "%ld repetitions in %lu msec." } ;
     CHAR        szBuffer [60] ;
     HPS         hps ;
     RECTL       rcl ;

     hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
     WinQueryWindowRect (hwnd, &rcl) ;

     sprintf (szBuffer, szMessage [sStatus], lCalcRep, ulTime) ;
     WinDrawText (hps, -1, szBuffer, &rcl, CLR_NEUTRAL, CLR_BACKGROUND,
                  DT_CENTER | DT_VCENTER | DT_ERASERECT) ;

     WinEndPaint (hps) ;
     }
