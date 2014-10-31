/*---------------------------------------
   FLOWER.C -- Transform Demonstration
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <math.h>

#define TWOPI (2 * 3.14159)

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Flower" ;
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

VOID DrawPetal (HPS hps)
     {
     static AREABUNDLE ab = { CLR_RED } ;
     static LINEBUNDLE lb = { CLR_BLACK } ;
     static POINTL     aptl[] = {   0,    0,  125,  125,  475, 125,  600, 0,
                                  475, -125,  125, -125,    0,   0 } ;

     GpiSavePS (hps) ;
     GpiSetAttrs (hps, PRIM_AREA, ABB_COLOR, 0, (PBUNDLE) &ab) ;
     GpiSetAttrs (hps, PRIM_LINE, LBB_COLOR, 0, (PBUNDLE) &lb) ;

     GpiBeginArea (hps, BA_BOUNDARY | BA_ALTERNATE) ;

     GpiMove (hps, aptl) ;
     GpiPolySpline (hps, 6, aptl + 1) ;

     GpiEndArea (hps) ;
     GpiRestorePS (hps, -1) ;
     }

VOID DrawPetals (HPS hps)
     {
     static AREABUNDLE ab = { CLR_YELLOW } ;
     static LINEBUNDLE lb = { CLR_BLACK } ;
     static POINTL     aptl [2] = { -150, -150,  150, 150 } ;
     INT               i ;
     MATRIXLF          matlf ;

     GpiSavePS (hps) ;

     GpiQueryModelTransformMatrix (hps, 9, &matlf) ;

     for (i = 0 ; i < 8 ; i++)
          {
          matlf.fxM11 = (FIXED) (65536 *  cos (TWOPI * i / 8)) ;
          matlf.fxM12 = (FIXED) (65536 *  sin (TWOPI * i / 8)) ;
          matlf.fxM21 = (FIXED) (65536 * -sin (TWOPI * i / 8)) ;
          matlf.fxM22 = (FIXED) (65536 *  cos (TWOPI * i / 8)) ;

          GpiSetModelTransformMatrix (hps, 9, &matlf, TRANSFORM_REPLACE) ;

          DrawPetal (hps) ;
          }

     GpiSetAttrs (hps, PRIM_AREA, ABB_COLOR, 0, (PBUNDLE) &ab) ;
     GpiSetAttrs (hps, PRIM_LINE, LBB_COLOR, 0, (PBUNDLE) &lb) ;

     GpiMove (hps, aptl) ;
     GpiBox (hps, DRO_OUTLINEFILL, aptl + 1, 300, 300) ;

     GpiRestorePS (hps, -1) ;
     }

VOID DrawFlower (HPS hps, INT cxClient, INT cyClient)
     {
     MATRIXLF matlf ;
     POINTL   ptl, aptl [4] ;

     GpiSavePS (hps) ;

     ptl.x = cxClient ;
     ptl.y = cyClient ;

     GpiConvert (hps, CVTC_DEVICE, CVTC_PAGE, 1, &ptl) ;

     aptl[0].x = 0 ;
     aptl[0].y = 0 ;

     aptl[1].x = 0 ;
     aptl[1].y = ptl.y / 4 ;

     aptl[2].x = 0 ;
     aptl[2].y = ptl.y ;

     aptl[3].x = ptl.x / 2 ;
     aptl[3].y = ptl.y / 2 ;

     GpiSavePS (hps) ;

     GpiSetLineWidthGeom (hps, 50) ;

     GpiBeginPath (hps, 1) ;
     GpiMove (hps, aptl) ;
     GpiPolySpline (hps, 3, aptl + 1) ;
     GpiEndPath (hps) ;

     GpiStrokePath (hps, 1, 0) ;

     GpiRestorePS (hps, -1) ;

     GpiQueryDefaultViewMatrix (hps, 9, &matlf) ;

     matlf.lM31 = ptl.x / 2 ;
     matlf.lM32 = ptl.y / 2 ;

     GpiSetDefaultViewMatrix (hps, 9, &matlf, TRANSFORM_REPLACE) ;

     DrawPetals (hps) ;

     GpiRestorePS (hps, -1) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static INT cxClient, cyClient ;
     HPS        hps ;
     SIZEL      sizel ;

     switch (msg)
          {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               sizel.cx = 0 ;
               sizel.cy = 0 ;
               GpiSetPS (hps, &sizel, PU_LOMETRIC) ;

               DrawFlower (hps, cxClient, cyClient) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
