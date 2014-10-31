/*---------------------------------------
   SKETCH.C -- Mouse Sketching Program
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

HAB  hab ;

int main (void)
     {
     static CHAR  szClientClass [] = "Sketch" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
				     0L, 0, 0, &hwndClient) ;

     if (hwndFrame == NULLHANDLE)
          WinMessageBox (HWND_DESKTOP, HWND_DESKTOP,
                         "Not enough memory to create the "
                         "bitmap used for storing images.",
                         szClientClass, 0, MB_OK | MB_WARNING) ;
     else
          {
          while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
               WinDispatchMsg (hab, &qmsg) ;

          WinDestroyWindow (hwndFrame) ;
          }

     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static BITMAPINFOHEADER2 bmp ;
     static BOOL              fButton1Down, fButton2Down ;
     static HBITMAP           hbm ;
     static HDC               hdcMemory ;
     static HPS               hpsMemory ;
     static POINTL            ptlPointerPos, aptl [3] ;
     HPS                      hpsWindow ;
     LONG                     cxFullScrn, cyFullScrn ;
     SIZEL                    sizl ;

     switch (msg)
          {
          case WM_CREATE:
               cxFullScrn = WinQuerySysValue (HWND_DESKTOP, SV_CXFULLSCREEN) ;
               cyFullScrn = WinQuerySysValue (HWND_DESKTOP, SV_CYFULLSCREEN) ;

                         // Create Memory DC and PS

               hdcMemory = DevOpenDC (hab, OD_MEMORY, "*", 0L, NULL, 0) ;

               sizl.cx = 0 ;
               sizl.cy = 0 ;
               hpsMemory = GpiCreatePS (hab, hdcMemory, &sizl,
					PU_PELS    | GPIF_DEFAULT |
                                        GPIT_MICRO | GPIA_ASSOC) ;

                         // Create monochrome bitmap, return 1 if cannot

               bmp.cbFix     = sizeof (BITMAPINFOHEADER2) ;
               bmp.cx        = cxFullScrn ;
               bmp.cy        = cyFullScrn ;
               bmp.cPlanes   = 1 ;
               bmp.cBitCount = 1 ;

               hbm = GpiCreateBitmap (hpsMemory, &bmp, 0L, 0L, NULL) ;

               if (hbm == 0)
                    {
                    GpiDestroyPS (hpsMemory) ;
                    DevCloseDC (hdcMemory) ;
                    return MRFROMSHORT (1) ;
                    }

                         // Set bitmap in memory PS and clear it

               GpiSetBitmap (hpsMemory, hbm) ;

               aptl[1].x = cxFullScrn ;
               aptl[1].y = cyFullScrn ;
               GpiBitBlt (hpsMemory, 0, 2L, aptl, ROP_ZERO, BBO_OR) ;
               return 0 ;

          case WM_BUTTON1DOWN:
               if (!fButton2Down)
                    WinSetCapture (HWND_DESKTOP, hwnd) ;

               ptlPointerPos.x = MOUSEMSG(&msg)->x ;
               ptlPointerPos.y = MOUSEMSG(&msg)->y ;

               fButton1Down = TRUE ;
               break ;                       // do default processing

          case WM_BUTTON1UP:
               if (!fButton2Down)
                    WinSetCapture (HWND_DESKTOP, NULLHANDLE) ;

               fButton1Down = FALSE ;
               return 0 ;

          case WM_BUTTON2DOWN:
               if (!fButton1Down)
                    WinSetCapture (HWND_DESKTOP, hwnd) ;

               ptlPointerPos.x = MOUSEMSG(&msg)->x ;
               ptlPointerPos.y = MOUSEMSG(&msg)->y ;

               fButton2Down = TRUE ;
               break ;                       // do default processing

          case WM_BUTTON2UP:
               if (!fButton1Down)
                    WinSetCapture (HWND_DESKTOP, NULLHANDLE) ;

               fButton2Down = FALSE ;
               return 0 ;

          case WM_MOUSEMOVE:
               if (!fButton1Down && !fButton2Down)
                    break ;

               hpsWindow = WinGetPS (hwnd) ;

               GpiSetColor (hpsMemory, fButton1Down ? CLR_TRUE : CLR_FALSE) ;
               GpiSetColor (hpsWindow,
                            fButton1Down ? CLR_NEUTRAL : CLR_BACKGROUND) ;

               GpiMove (hpsMemory, &ptlPointerPos) ;
               GpiMove (hpsWindow, &ptlPointerPos) ;

               ptlPointerPos.x = MOUSEMSG(&msg)->x ;
               ptlPointerPos.y = MOUSEMSG(&msg)->y ;

               GpiLine (hpsMemory, &ptlPointerPos) ;
               GpiLine (hpsWindow, &ptlPointerPos) ;

               WinReleasePS (hpsWindow) ;
               break ;                       // do default processing

          case WM_PAINT:
               hpsWindow = WinBeginPaint (hwnd, NULLHANDLE, (PRECTL) aptl) ;

               aptl[2] = aptl[0] ;

               GpiBitBlt (hpsWindow, hpsMemory, 3L, aptl, ROP_SRCCOPY,
                          BBO_OR) ;

               WinEndPaint (hpsWindow) ;
               return 0 ;

          case WM_DESTROY:
               GpiDestroyPS (hpsMemory) ;
               DevCloseDC (hdcMemory) ;
               GpiDeleteBitmap (hbm) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
