/*---------------------------------------
   BLOWUP.C -- Screen Capture Program
               (c) Charles Petzold, 1993
  ---------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>
#include "blowup.h"

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
BOOL             BeginTracking (PRECTL) ;
HBITMAP          CopyScreenToBitmap (PRECTL) ;
HBITMAP          CopyBitmap (HBITMAP) ;
VOID             BitmapCreationError (HWND) ;

CHAR szClientClass [] = "BlowUp" ;
HAB  hab ;

int main (void)
     {
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST |
                                 FCF_MENU          | FCF_ACCELTABLE ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, 0, ID_RESOURCE, &hwndClient) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static HBITMAP hbm ;
     static HWND    hwndMenu ;
     static INT     iDisplay = IDM_ACTUAL ;
     BOOL           bEnable ;
     HBITMAP        hbmClip ;
     HPS            hps ;
     RECTL          rclTrack, rclClient ;
     ULONG          ulfInfo ;

     switch (msg)
          {
          case WM_CREATE:
               hwndMenu = WinWindowFromID (
                               WinQueryWindow (hwnd, QW_PARENT),
                               FID_MENU) ;
               return 0 ;

          case WM_INITMENU:
               switch (SHORT1FROMMP (mp1))
                    {
                    case IDM_EDIT:
                         bEnable = (hbm != NULLHANDLE ? TRUE : FALSE) ;

                         WinEnableMenuItem (hwndMenu, IDM_CUT,   bEnable) ;
                         WinEnableMenuItem (hwndMenu, IDM_COPY,  bEnable) ;
                         WinEnableMenuItem (hwndMenu, IDM_CLEAR, bEnable) ;
                         WinEnableMenuItem (hwndMenu, IDM_PASTE,
                              WinQueryClipbrdFmtInfo (hab, CF_BITMAP,
                                                      &ulfInfo)) ;
                         return 0 ;
                    }
               break ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_CUT:
                         if (hbm != NULLHANDLE)
                              {
                              WinOpenClipbrd (hab) ;
                              WinEmptyClipbrd (hab) ;
                              WinSetClipbrdData (hab, (ULONG) hbm,
                                                 CF_BITMAP, CFI_HANDLE) ;
                              WinCloseClipbrd (hab) ;
                              hbm = NULLHANDLE ;
                              WinInvalidateRect (hwnd, NULL, FALSE) ;
                              }
                         return 0 ;

                    case IDM_COPY:
                                        // Make copy of stored bitmap

                         hbmClip = CopyBitmap (hbm) ;

                                        // Set clipboard data to copy of bitmap

                         if (hbmClip != NULLHANDLE)
                              {
                              WinOpenClipbrd (hab) ;
                              WinEmptyClipbrd (hab) ;
                              WinSetClipbrdData (hab, (ULONG) hbmClip,
                                                 CF_BITMAP, CFI_HANDLE) ;
                              WinCloseClipbrd (hab) ;
                              }
                         else
                              BitmapCreationError (hwnd) ;
                         return 0 ;

                    case IDM_PASTE:
                                         // Get bitmap from clipboard

                         WinOpenClipbrd (hab) ;
                         hbmClip = (HBITMAP) WinQueryClipbrdData (hab,
                                                                  CF_BITMAP) ;
                         if (hbmClip != NULLHANDLE)
                              {
                              if (hbm != NULLHANDLE)
                                   GpiDeleteBitmap (hbm) ;

                                        // Make copy of clipboard bitmap

                              hbm = CopyBitmap (hbmClip) ;

                              if (hbm == NULLHANDLE)
                                   BitmapCreationError (hwnd) ;
                              }
                         WinCloseClipbrd (hab) ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;

                    case IDM_CLEAR:
                         if (hbm != NULLHANDLE)
                              {
                              GpiDeleteBitmap (hbm) ;
                              hbm = NULLHANDLE ;
                              WinInvalidateRect (hwnd, NULL, FALSE) ;
                              }
                         return 0 ;

                    case IDM_CAPTURE:
                         if (BeginTracking (&rclTrack))
                              {
                              if (hbm != NULLHANDLE)
                                   GpiDeleteBitmap (hbm) ;

                              hbm = CopyScreenToBitmap (&rclTrack) ;

                              if (hbm == NULLHANDLE)
                                   BitmapCreationError (hwnd) ;

                              WinInvalidateRect (hwnd, NULL, FALSE) ;
                              }
                         return 0 ;

                    case IDM_ACTUAL:
                    case IDM_STRETCH:
                         WinCheckMenuItem (hwndMenu, iDisplay, FALSE) ;

                         iDisplay = COMMANDMSG(&msg)->cmd ;

                         WinCheckMenuItem (hwndMenu, iDisplay, TRUE) ;
                         WinInvalidateRect (hwnd, NULL, FALSE) ;
                         return 0 ;
                    }
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               GpiErase (hps) ;

               if (hbm != NULLHANDLE)
                    {
                    WinQueryWindowRect (hwnd, &rclClient) ;

                    WinDrawBitmap (hps, hbm, NULL, (PPOINTL) &rclClient,
                                   CLR_NEUTRAL, CLR_BACKGROUND,
                                   iDisplay == IDM_STRETCH ?
                                        DBM_STRETCH : DBM_NORMAL) ;
                    }
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               if (hbm != NULLHANDLE)
                    GpiDeleteBitmap (hbm) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

BOOL BeginTracking (PRECTL prclTrack)
     {
     INT       cxScreen, cyScreen, cxPointer, cyPointer ;
     TRACKINFO ti ;

     cxScreen  = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN) ;
     cyScreen  = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN) ;
     cxPointer = WinQuerySysValue (HWND_DESKTOP, SV_CXPOINTER) ;
     cyPointer = WinQuerySysValue (HWND_DESKTOP, SV_CYPOINTER) ;

                                   // Set up track rectangle for moving

     ti.cxBorder = 1 ;                       // Border width
     ti.cyBorder = 1 ;
     ti.cxGrid = 0 ;                         // Not used
     ti.cyGrid = 0 ;
     ti.cxKeyboard = 4 ;                     // Pixel increment for keyboard
     ti.cyKeyboard = 4 ;

     ti.rclBoundary.xLeft   = 0 ;            // Area for tracking rectangle
     ti.rclBoundary.yBottom = 0 ;
     ti.rclBoundary.xRight  = cxScreen ;
     ti.rclBoundary.yTop    = cyScreen ;

     ti.ptlMinTrackSize.x = 1 ;              // Minimum rectangle size
     ti.ptlMinTrackSize.y = 1 ;

     ti.ptlMaxTrackSize.x = cxScreen ;       // Maximum rectangle size
     ti.ptlMaxTrackSize.y = cyScreen ;
                                             // Initial position

     ti.rclTrack.xLeft   = (cxScreen - cxPointer) / 2 ;
     ti.rclTrack.yBottom = (cyScreen - cyPointer) / 2 ;
     ti.rclTrack.xRight  = (cxScreen + cxPointer) / 2 ;
     ti.rclTrack.yTop    = (cyScreen + cyPointer) / 2 ;

     ti.fs = TF_MOVE | TF_STANDARD | TF_SETPOINTERPOS ;     // Flags

     if (!WinTrackRect (HWND_DESKTOP, NULLHANDLE, &ti))
          return FALSE ;
                                   // Switch to "sizing" pointer
     WinSetPointer (HWND_DESKTOP,
               WinQuerySysPointer (HWND_DESKTOP, SPTR_SIZENESW, FALSE)) ;

                                   // Track rectangle for sizing

     ti.fs = TF_RIGHT | TF_TOP | TF_STANDARD | TF_SETPOINTERPOS ;

     if (!WinTrackRect (HWND_DESKTOP, NULLHANDLE, &ti))
          return FALSE ;

     *prclTrack = ti.rclTrack ;    // Final rectangle

     return TRUE ;
     }

HBITMAP CopyScreenToBitmap (PRECTL prclTrack)
     {
     BITMAPINFOHEADER2 bmp ;
     HBITMAP           hbm ;
     HDC               hdcMemory ;
     HPS               hps, hpsMemory ;
     LONG              alBmpFormats[2] ;
     POINTL            aptl[3] ;
     SIZEL             sizl ;

                                   // Create memory DC and PS

     hdcMemory = DevOpenDC (hab, OD_MEMORY, "*", 0L, NULL, NULLHANDLE) ;

     sizl.cx = sizl.cy = 0 ;
     hpsMemory = GpiCreatePS (hab, hdcMemory, &sizl,
                              PU_PELS    | GPIF_DEFAULT |
                              GPIT_MICRO | GPIA_ASSOC) ;

                                   // Create bitmap for destination

     GpiQueryDeviceBitmapFormats (hpsMemory, 2L, alBmpFormats) ;

     memset (&bmp, 0, sizeof (BITMAPINFOHEADER2)) ;

     bmp.cbFix     = sizeof (BITMAPINFOHEADER2) ;
     bmp.cx        = prclTrack->xRight - prclTrack->xLeft ;
     bmp.cy        = prclTrack->yTop   - prclTrack->yBottom ;
     bmp.cPlanes   = (USHORT) alBmpFormats[0] ;
     bmp.cBitCount = (USHORT) alBmpFormats[1] ;

     hbm = GpiCreateBitmap (hpsMemory, &bmp, 0L, NULL, NULL) ;

                                   // Copy from screen to bitmap
     if (hbm != NULLHANDLE)
          {
          GpiSetBitmap (hpsMemory, hbm) ;
          hps = WinGetScreenPS (HWND_DESKTOP) ;

          aptl[0].x = 0 ;
          aptl[0].y = 0 ;
          aptl[1].x = bmp.cx ;
          aptl[1].y = bmp.cy ;
          aptl[2].x = prclTrack->xLeft ;
          aptl[2].y = prclTrack->yBottom ;

          WinLockVisRegions (HWND_DESKTOP, TRUE) ;

          GpiBitBlt (hpsMemory, hps, 3L, aptl, ROP_SRCCOPY, BBO_IGNORE);

          WinLockVisRegions (HWND_DESKTOP, FALSE) ;

          WinReleasePS (hps) ;
          }
                                   // Clean up
     GpiDestroyPS (hpsMemory) ;
     DevCloseDC (hdcMemory) ;

     return hbm ;
     }

HBITMAP CopyBitmap (HBITMAP hbmSrc)
     {
     BITMAPINFOHEADER2 bmp ;
     HBITMAP           hbmDst ;
     HDC               hdcSrc, hdcDst ;
     HPS               hpsSrc, hpsDst ;
     POINTL            aptl[3] ;
     SIZEL             sizl ;

                                   // Create memory DC's and PS's

     hdcSrc = DevOpenDC (hab, OD_MEMORY, "*", 0L, NULL, NULLHANDLE) ;
     hdcDst = DevOpenDC (hab, OD_MEMORY, "*", 0L, NULL, NULLHANDLE) ;

     sizl.cx = sizl.cy = 0 ;
     hpsSrc = GpiCreatePS (hab, hdcSrc, &sizl, PU_PELS    | GPIF_DEFAULT |
                                               GPIT_MICRO | GPIA_ASSOC) ;

     hpsDst = GpiCreatePS (hab, hdcDst, &sizl, PU_PELS    | GPIF_DEFAULT |
                                               GPIT_MICRO | GPIA_ASSOC) ;

                                   // Create bitmap

     bmp.cbFix = sizeof (BITMAPINFOHEADER2) ;
     GpiQueryBitmapInfoHeader (hbmSrc, &bmp) ;
     hbmDst = GpiCreateBitmap (hpsDst, &bmp, 0L, NULL, NULL) ;

                                   // Copy from source to destination

     if (hbmDst != NULLHANDLE)
          {
          GpiSetBitmap (hpsSrc, hbmSrc) ;
          GpiSetBitmap (hpsDst, hbmDst) ;

          aptl[0].x = aptl[0].y = 0 ;
          aptl[1].x = bmp.cx ;
          aptl[1].y = bmp.cy ;
          aptl[2]   = aptl[0] ;

          GpiBitBlt (hpsDst, hpsSrc, 3L, aptl, ROP_SRCCOPY, BBO_IGNORE) ;
          }
                                   // Clean up
     GpiDestroyPS (hpsSrc) ;
     GpiDestroyPS (hpsDst) ;
     DevCloseDC (hdcSrc) ;
     DevCloseDC (hdcDst) ;

     return hbmDst ;
     }

VOID BitmapCreationError (HWND hwnd)
     {
     WinMessageBox (HWND_DESKTOP, hwnd, "Cannot create bitmap.",
                    szClientClass, 0, MB_OK | MB_WARNING) ;
     }
