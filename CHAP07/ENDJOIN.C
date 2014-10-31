/*----------------------------------------
   ENDJOIN.C -- Line Ends and Joins
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "EndJoin" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
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

void DrawFigure (HPS hps, INT i, INT cxClient, INT cyClient)
     {
     POINTL ptl ;

     ptl.x = (1 + 10 * i) * cxClient / 30 ;
     ptl.y = 3 * cyClient / 4 ;
     GpiMove (hps, &ptl) ;

     ptl.x = (5 + 10 * i) * cxClient / 30 ;
     ptl.y = cyClient / 4 ;
     GpiLine (hps, &ptl) ;

     ptl.x = (9 + 10 * i) * cxClient / 30 ;
     ptl.y = 3 * cyClient / 4 ;
     GpiLine (hps, &ptl) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static INT  cxClient, cyClient ;
     static LONG alJoin [] = { LINEJOIN_BEVEL, LINEJOIN_ROUND, LINEJOIN_MITRE },
                 alEnd  [] = { LINEEND_FLAT, LINEEND_SQUARE, LINEEND_ROUND } ;
     HPS         hps ;
     INT         i ;

     switch (msg)
	  {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               GpiErase (hps) ;

               for (i = 0 ; i < 3 ; i++)
                    {
                              // Draw the geometric line

                    GpiSetLineJoin (hps, alJoin [i]) ;
                    GpiSetLineEnd  (hps, alEnd  [i]) ;
                    GpiSetLineWidthGeom (hps, cxClient / 20) ;
                    GpiSetColor (hps, CLR_DARKGRAY) ;

                    GpiBeginPath (hps, 1) ;
                    DrawFigure (hps, i, cxClient, cyClient) ;
                    GpiEndPath (hps) ;

                    GpiStrokePath (hps, 1, 0) ;

                              // Draw the cosmetic line

                    GpiSetLineWidth (hps, LINEWIDTH_THICK) ;
                    GpiSetColor (hps, CLR_BLACK) ;

                    DrawFigure (hps, i, cxClient, cyClient) ;
                    }

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
