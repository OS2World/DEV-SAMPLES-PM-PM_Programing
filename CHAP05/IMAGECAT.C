/*-----------------------------------------
   IMAGECAT.C -- Cat drawn using GpiImage
                 (c) Charles Petzold, 1993
  -----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "ImageCat" ;
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
     static BYTE abCat [] = {
                            0x01, 0xF8, 0x1F, 0x80, 0x01, 0x04, 0x20, 0x80,
                            0x00, 0x8F, 0xF1, 0x00, 0x00, 0x48, 0x12, 0x00,
                            0x00, 0x28, 0x14, 0x00, 0x00, 0x1A, 0x58, 0x00,
                            0x00, 0x08, 0x10, 0x00, 0x00, 0xFC, 0x3F, 0x00,
                            0x00, 0x09, 0x90, 0x00, 0x00, 0xFC, 0x3F, 0x00,
                            0x00, 0x08, 0x10, 0x00, 0x00, 0x07, 0xE0, 0x00,
                            0x00, 0x08, 0x10, 0x00, 0x00, 0x08, 0x10, 0xC0,
                            0x00, 0x08, 0x10, 0x20, 0x00, 0x10, 0x08, 0x10,
                            0x00, 0x10, 0x08, 0x08, 0x00, 0x10, 0x08, 0x04,
                            0x00, 0x20, 0x04, 0x04, 0x00, 0x20, 0x04, 0x04,
                            0x00, 0x20, 0x04, 0x04, 0x00, 0x40, 0x02, 0x04,
                            0x00, 0x40, 0x02, 0x04, 0x00, 0x40, 0x02, 0x04,
                            0x00, 0xC0, 0x03, 0x04, 0x00, 0x9C, 0x39, 0x08,
                            0x00, 0xA2, 0x45, 0x08, 0x00, 0xA2, 0x45, 0x10,
                            0x00, 0xA2, 0x45, 0xE0, 0x00, 0xA2, 0x45, 0x00,
                            0x00, 0xA2, 0x45, 0x00, 0x00, 0xFF, 0xFF, 0x00 } ;
     static INT  cxClient, cyClient ;
     HPS         hps ;
     POINTL      ptl ;
     SIZEL       sizl ;

     switch (msg)
          {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               ptl.x = cxClient / 2 - 16 ;
               ptl.y = cyClient / 2 + 16 ;
               GpiMove (hps, &ptl) ;

               sizl.cx = 32 ;
               sizl.cy = 32 ;
               GpiImage (hps, 0L, &sizl, sizeof abCat, abCat) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
