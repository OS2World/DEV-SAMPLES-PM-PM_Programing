/*--------------------------------------------
   DRAWLINE.C -- Draw line from radio buttons
                 (c) Charles Petzold, 1993
  --------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "DrawLine" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
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

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CHAR   *szGroupText[] = { "Color", "Type" } ;
     static CHAR   *szColorText[] = { "Background", "Blue",      "Red",
                                      "Pink",       "Green",     "Cyan",
                                      "Yellow",     "Neutral",   "Dark Gray",
                                      "Dark Blue",  "Dark Red",  "Dark Pink",
                                      "Dark Green", "Dark Cyan", "Brown",
                                      "Pale Gray" } ;
     static CHAR   *szTypeText [] = { "Dot",        "Short Dash",
                                      "Dash Dot",   "Double Dot",
                                      "Long Dash",  "Dash Double Dot",
                                      "Solid",      "Invisible" } ;
     static HWND   hwndGroup[2], hwndRadioColor[16], hwndRadioType[8] ;
     static INT    iCurrentColor = 7,   // Neutral
                   iCurrentType  = 6 ;  // Solid
     static POINTL aptl[5] ;
     FONTMETRICS   fm ;
     HPS           hps ;
     INT           i, id, cxChar, cyChar ;

     switch (msg)
          {
          case WM_CREATE :
               hps = WinGetPS (hwnd) ;
               GpiQueryFontMetrics (hps, sizeof fm, &fm) ;
               cxChar = fm.lAveCharWidth ;
               cyChar = fm.lMaxBaselineExt ;
               WinReleasePS (hps) ;

               for (i = 0 ; i < 2 ; i++)

                    hwndGroup[i] = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_STATIC,          // Class
                                        szGroupText[i],     // Text
                                        WS_VISIBLE |        // Style
                                             SS_GROUPBOX,
                                        (8 + 42 * i) * cxChar,
                                        4 * cyChar,         // Position
                                        (26 + 12 * (1 - i)) *
                                             cxChar,        // Width
                                        14 * cyChar,        // Height
                                        hwnd,               // Owner
                                        HWND_TOP,           // Placement
                                        i + 24,             // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params

               for (i = 0 ; i < 16 ; i++)

                    hwndRadioColor[i] = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_BUTTON,          // Class
                                        szColorText[i],     // Text
                                        WS_VISIBLE |        // Style
                                             BS_RADIOBUTTON,
                                        (10 + (i > 7 ? 18 : 0))
                                             * cxChar,      // X Position
                                        (31 - 3 * (i % 8))
                                             * cyChar / 2,  // Y Position
                                        16 * cxChar,        // Width
                                        3 * cyChar / 2,     // Height
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        i,                  // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params

               for (i = 0 ; i < 8 ; i++)

                    hwndRadioType[i]  = WinCreateWindow (
                                        hwnd,               // Parent
                                        WC_BUTTON,          // Class
                                        szTypeText[i],      // Text
                                        WS_VISIBLE |        // Style
                                             BS_RADIOBUTTON,
                                        52 * cxChar,        // Position
                                        (31 - 3 * i) * cyChar / 2,
                                        22 * cxChar,        // Width
                                        3 * cyChar / 2,     // Height
                                        hwnd,               // Owner
                                        HWND_BOTTOM,        // Placement
                                        i + 16,             // ID
                                        NULL,               // Ctrl Data
                                        NULL) ;             // Pres Params
                    
               WinSendMsg (hwndRadioColor[iCurrentColor],
                           BM_SETCHECK, MPFROMSHORT (1), NULL) ;

               WinSendMsg (hwndRadioType[iCurrentType],
                           BM_SETCHECK, MPFROMSHORT (1), NULL) ;

               aptl[0].x = aptl[3].x = aptl[4].x = 4 * cxChar ;
               aptl[1].x = aptl[2].x = 80 * cxChar ;

               aptl[0].y = aptl[1].y = aptl[4].y = 2 * cyChar ;
               aptl[2].y = aptl[3].y = 20 * cyChar ;

               return 0 ;

          case WM_CONTROL:
               id = SHORT1FROMMP (mp1) ;

               if (id < 16)             // Color IDs
                    {
                    WinSendMsg (hwndRadioColor[iCurrentColor],
                                BM_SETCHECK, MPFROMSHORT (0), NULL) ;

                    iCurrentColor = id ;

                    WinSendMsg (hwndRadioColor[iCurrentColor],
                                BM_SETCHECK, MPFROMSHORT (1), NULL) ;
                    }

               else if (id < 24)        // Line Type IDs
                    {
                    WinSendMsg (hwndRadioType[iCurrentType],
                                BM_SETCHECK, MPFROMSHORT (0), NULL) ;

                    iCurrentType = id - 16 ;

                    WinSendMsg (hwndRadioType[iCurrentType],
                                BM_SETCHECK, MPFROMSHORT (1), NULL) ;
                    }
               WinInvalidateRect (hwnd, NULL, TRUE) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               GpiSetColor (hps, iCurrentColor) ;
               GpiSetLineType (hps, iCurrentType + LINETYPE_DOT) ;
               GpiMove (hps, aptl) ;
               GpiPolyLine (hps, 4L, aptl + 1) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
