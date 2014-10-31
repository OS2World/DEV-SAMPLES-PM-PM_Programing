/*---------------------------------------
   COLORS.C -- GPI Foreground Colors
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Colors" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
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

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static struct {
                   LONG  lColorIndex ;
                   PCHAR szColorIndex ;
                   }
                   show [] = 
                   {
                   CLR_FALSE      , "CLR_FALSE"      , 
                   CLR_TRUE       , "CLR_TRUE"       , 
                   CLR_DEFAULT    , "CLR_DEFAULT"    , 
                   CLR_WHITE      , "CLR_WHITE"      , 
                   CLR_BLACK      , "CLR_BLACK"      , 
                   CLR_BACKGROUND , "CLR_BACKGROUND" , 
                   CLR_BLUE       , "CLR_BLUE"       , 
                   CLR_RED        , "CLR_RED"        , 
                   CLR_PINK       , "CLR_PINK"       , 
                   CLR_GREEN      , "CLR_GREEN"      , 
                   CLR_CYAN       , "CLR_CYAN"       , 
                   CLR_YELLOW     , "CLR_YELLOW"     , 
                   CLR_NEUTRAL    , "CLR_NEUTRAL"    , 
                   CLR_DARKGRAY   , "CLR_DARKGRAY"   , 
                   CLR_DARKBLUE   , "CLR_DARKBLUE"   ,
                   CLR_DARKRED    , "CLR_DARKRED"    ,
                   CLR_DARKPINK   , "CLR_DARKPINK"   ,
                   CLR_DARKGREEN  , "CLR_DARKGREEN"  , 
                   CLR_DARKCYAN   , "CLR_DARKCYAN"   , 
                   CLR_BROWN      , "CLR_BROWN"      , 
                   CLR_PALEGRAY   , "CLR_PALEGRAY"
                   } ;
     static INT    cyClient, cxCaps, cyChar, cyDesc,
                   iNumColors = sizeof show / sizeof show[0] ;
     FONTMETRICS   fm ;
     HPS           hps ;
     INT           i ;
     POINTL        ptl ;

     switch (msg)
          {
          case WM_CREATE:
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxCaps = (fm.fsType & 1 ? 2 : 3) * fm.lAveCharWidth / 2 ;
               cyChar = fm.lMaxBaselineExt ;
               cyDesc = fm.lMaxDescender ;
               WinReleasePS (hps) ;
               return 0 ;

          case WM_SIZE:
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               for (i = 0 ; i < iNumColors ; i ++)
                    {
                    ptl.x = (i < 11 ? 1 : 33) * cxCaps ;
                    ptl.y = cyClient - (i % 11 * 5 + 4) * cyChar / 2 + cyDesc ;

                    GpiCharStringAt (hps, &ptl,
                                     strlen (show [i].szColorIndex),
                                     show [i].szColorIndex) ;

                    ptl.x  = (i < 11 ? 20 : 52) * cxCaps ;
       	            ptl.y -= cyDesc + cyChar / 2 ;
                    GpiMove (hps, &ptl) ;

                    GpiSavePS (hps) ;
                    GpiSetColor (hps, show [i].lColorIndex) ;

                    ptl.x += 10 * cxCaps ;
                    ptl.y +=  2 * cyChar ;
                    GpiBox (hps, DRO_FILL, &ptl, 0L, 0L) ;

                    GpiRestorePS (hps, -1L) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
