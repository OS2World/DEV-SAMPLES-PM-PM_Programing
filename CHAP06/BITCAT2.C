/*------------------------------------------
   BITCAT2.C -- Bitmap Creation and Display
                (c) Charles Petzold, 1993
  ------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include "bitcat.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "BitCat2" ;
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

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static HBITMAP      hbm ;
     static HDC          hdcMemory ;
     static HPS          hpsMemory ;
     static INT          cxClient, cyClient ;
     BITMAPINFO2       * pbmi ;
     BITMAPINFOHEADER2   bmp ;
     HAB                 hab ;
     HPS                 hps ;
     POINTL              aptl [4] ;
     SIZEL               sizl ;

     switch (msg)
          {
          case WM_CREATE:
               hab = WinQueryAnchorBlock (hwnd) ;

                         // Open memory DC and create PS associated with it

               hdcMemory = DevOpenDC (hab, OD_MEMORY, "*", 0L, NULL, 0) ;

               sizl.cx = 0 ;
               sizl.cy = 0 ;

               hpsMemory = GpiCreatePS (hab, hdcMemory, &sizl,
					PU_PELS    | GPIF_DEFAULT |
                                        GPIT_MICRO | GPIA_ASSOC) ;

                         // Create 32 by 32 bitmap

               memset (&bmp, 0, sizeof (BITMAPINFOHEADER2)) ;

               bmp.cbFix     = sizeof (BITMAPINFOHEADER2) ;
               bmp.cx        = 32 ;
               bmp.cy        = 32 ;
               bmp.cPlanes   = 1 ;
               bmp.cBitCount = 1 ;

               hbm = GpiCreateBitmap (hpsMemory, &bmp, 0L, NULL, NULL) ;

                         // Select bitmap into memory PS

               GpiSetBitmap (hpsMemory, hbm) ;

                         // Set bitmap bits from abBitCat array

               pbmi = malloc (sizeof (BITMAPINFO2) + sizeof (RGB)) ;

               memset (pbmi, 0, sizeof (BITMAPINFOHEADER2)) ;

               pbmi->cbFix     = sizeof (BITMAPINFOHEADER2) ;
               pbmi->cx        = 32 ;
               pbmi->cy        = 32 ;
               pbmi->cPlanes   = 1 ;
               pbmi->cBitCount = 1 ;

               pbmi->argbColor[0].bBlue  = 0x00 ;      // 0 bits (background)
               pbmi->argbColor[0].bGreen = 0x00 ;
               pbmi->argbColor[0].bRed   = 0x00 ;
               pbmi->argbColor[0].fcOptions = 0 ;

               pbmi->argbColor[1].bBlue  = 0xFF ;      // 1 bits (foreground)
               pbmi->argbColor[1].bGreen = 0xFF ;
               pbmi->argbColor[1].bRed   = 0xFF ;
               pbmi->argbColor[0].fcOptions = 0 ;

               GpiSetBitmapBits (hpsMemory, 0L, 32L, abBitCat, pbmi) ;

               free (pbmi) ;
               return 0 ;

          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               aptl[0].x = 0 ;                    // target lower left
               aptl[0].y = 0 ;

               aptl[1].x = cxClient ;             // target upper right
               aptl[1].y = cyClient ;

               aptl[2].x = 0 ;                    // source lower left
               aptl[2].y = 0 ;

               aptl[3].x = 32 ;                   // source upper right
               aptl[3].y = 32 ;

               GpiBitBlt (hps, hpsMemory, 4L, aptl, ROP_SRCCOPY, BBO_AND) ;

               aptl[1] = aptl[3] ;                // target upper right

               GpiBitBlt (hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, BBO_AND) ;

               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               GpiDestroyPS (hpsMemory) ;
               DevCloseDC (hdcMemory) ;
               GpiDeleteBitmap (hbm) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
