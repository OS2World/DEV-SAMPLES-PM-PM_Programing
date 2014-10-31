/*----------------------------------------
   FREEMEM.C -- Free Memory Display
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#include <os2.h>
#include <string.h>

#define ID_TIMER 1

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
VOID    SizeTheWindow (HWND) ;

int main (void)
     {
     static CHAR  szClientClass[] = "FreeMem" ;
     static ULONG flFrameFlags = FCF_TITLEBAR | FCF_SYSMENU  |
                                 FCF_BORDER   | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, 0L, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
				     0L, 0, 0, &hwndClient) ;
     SizeTheWindow (hwndFrame) ;

     if (WinStartTimer (hab, hwndClient, ID_TIMER, 1000))
          {
          while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
               WinDispatchMsg (hab, &qmsg) ;

          WinStopTimer (hab, hwndClient, ID_TIMER) ;
          }
     else
          WinMessageBox (HWND_DESKTOP, hwndClient,
                         "Too many clocks or timers",
                         szClientClass, 0, MB_OK | MB_WARNING) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

VOID SizeTheWindow (HWND hwndFrame)
     {
     static CHAR szText [] = "1,234,567,890 bytes" ;
     HPS         hps ;
     POINTL      aptl[TXTBOX_COUNT] ;
     RECTL       rcl ;

     hps = WinGetPS (hwndFrame) ;
     GpiQueryTextBox (hps, sizeof szText - 1L, szText, TXTBOX_COUNT, aptl) ;
     WinReleasePS (hps) ;

     rcl.yBottom = 0 ;
     rcl.yTop    = 3 * (aptl[TXTBOX_TOPLEFT].y -
                        aptl[TXTBOX_BOTTOMLEFT].y) / 2 ;
     rcl.xLeft   = 0 ;
     rcl.xRight  = (sizeof szText + 1L) * (aptl[TXTBOX_BOTTOMRIGHT].x -
                   aptl[TXTBOX_BOTTOMLEFT].x) / (sizeof szText - 1L) ;

     WinCalcFrameRect (hwndFrame, &rcl, FALSE) ;

     WinSetWindowPos (hwndFrame, NULLHANDLE, rcl.xLeft, rcl.yBottom,
                      rcl.xRight - rcl.xLeft, rcl.yTop - rcl.yBottom,
                      SWP_SIZE | SWP_MOVE) ;
     }

VOID FormatNumber (CHAR *pchResult, ULONG ulValue)
     {
     BOOL  fDisplay = FALSE ;
     INT   iDigit ;
     ULONG ulQuotient, ulDivisor = 1000000000L ;

     for (iDigit = 0 ; iDigit < 10 ; iDigit++)
          {
          ulQuotient = ulValue / ulDivisor ;

          if (fDisplay || ulQuotient > 0 || iDigit == 9)
               {
               fDisplay = TRUE ;

               *pchResult++ = (CHAR) ('0' + ulQuotient) ;

               if ((iDigit % 3 == 0) && iDigit != 9)
                    *pchResult++ = ',' ;
               }
          ulValue -= ulQuotient * ulDivisor ;
          ulDivisor /= 10 ;
          }
     *pchResult = '\0' ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static RECTL rcl ;
     static ULONG ulFreeMem, ulPrevMem ;
     CHAR         szBuffer [24] ;
     HPS          hps;

     switch (msg)
          {
          case WM_SIZE:
               WinQueryWindowRect (hwnd, &rcl) ;
               return 0 ;               

          case WM_TIMER:
               DosQuerySysInfo (QSV_TOTAVAILMEM, QSV_TOTAVAILMEM,
                                (PBYTE) &ulFreeMem, sizeof (ULONG)) ;

               if (ulFreeMem != ulPrevMem)
                    {
                    WinInvalidateRect (hwnd, NULL, FALSE) ;
                    ulPrevMem = ulFreeMem ;
                    }
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               FormatNumber (szBuffer, ulFreeMem) ;
               strcat (szBuffer, " bytes") ;

               WinDrawText (hps, -1, szBuffer, &rcl, 
                            CLR_NEUTRAL, CLR_BACKGROUND,
                            DT_CENTER | DT_VCENTER | DT_ERASERECT) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
