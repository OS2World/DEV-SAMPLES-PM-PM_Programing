/*---------------------------------------
   SPIRAL.C -- GPI Spiral Drawing
               (c) Charles Petzold, 1993
 ----------------------------------------*/

#include <os2.h>
#include <math.h>
#include <stdlib.h>

#define NUMPOINTS 1000
#define NUMREV    20
#define PI        3.14159

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Spiral" ;
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
     static INT cxClient, cyClient ;
     double     dAngle, dScale ;
     HPS        hps ;
     INT        i ;
     PPOINTL    pptl ;

     switch (msg)
	  {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               if ((pptl = malloc (NUMPOINTS * sizeof (POINTL))) != NULL)
                    {
                    for (i = 0 ; i < NUMPOINTS ; i ++)
                        {
                        dAngle = i * 2 * PI / (NUMPOINTS / NUMREV) ;
                        dScale = 1 - (double) i / NUMPOINTS ;

                        pptl[i].x = (LONG) (cxClient / 2 *
                                                 (1 + dScale * cos (dAngle))) ;

                        pptl[i].y = (LONG) (cyClient / 2 *
                                                 (1 + dScale * sin (dAngle))) ;
                        }
                    GpiMove (hps, pptl) ;
                    GpiPolyLine (hps, NUMPOINTS - 1L, pptl + 1) ;

                    free (pptl) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
