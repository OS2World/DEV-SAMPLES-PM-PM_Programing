/*-------------------------------------------
   OUTFONTS.C -- Displays OS/2 Outline Fonts
                 (c) Charles Petzold, 1993
  -------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LCID_FONT    1
#define PTSIZE      12

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "OutFonts" ;
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

LONG GetAllOutlineFonts (HPS hps, PFONTMETRICS * ppfm)
     {
     LONG         l, lAllFnt, lOutFnt ;
     PFONTMETRICS pfmAll, pfmOut ;

               // Find number of fonts

     lAllFnt = 0 ;
     lAllFnt = GpiQueryFonts (hps, QF_PUBLIC, NULL, &lAllFnt, 0, NULL) ;

     if (lAllFnt ==  0)
          return 0 ;

               // Allocate memory for FONTMETRICS structures

     pfmAll = (PFONTMETRICS) calloc (lAllFnt, sizeof (FONTMETRICS)) ;

     if (pfmAll == NULL)
          return 0 ;

     pfmOut = (PFONTMETRICS) calloc (lAllFnt, sizeof (FONTMETRICS)) ;

     if (pfmOut == NULL)
          {
          free (pfmAll) ;
          return 0 ;
          }
               // Get all fonts

     GpiQueryFonts (hps, QF_PUBLIC, NULL, &lAllFnt,
                         sizeof (FONTMETRICS), pfmAll) ;

               // Get all the outline fonts

     lOutFnt = 0 ;

     for (l = 0 ; l < lAllFnt ; l++)
          if (pfmAll[l].fsDefn & FM_DEFN_OUTLINE)
               pfmOut [lOutFnt ++] = pfmAll [l] ;

               // Clean up

     free (pfmAll) ;
     pfmOut = (PFONTMETRICS) realloc (pfmOut, lOutFnt * sizeof (FONTMETRICS)) ;

     * ppfm = pfmOut ;

     return lOutFnt ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static FATTRS       fat ;
     static LONG         xRes, yRes, lFonts ;
     static PFONTMETRICS pfm ;
     static SHORT        cyClient ;
     CHAR                szBuffer [FACESIZE + 32] ;
     FONTMETRICS         fm ;
     HDC                 hdc ;
     HPS                 hps ;
     LONG                l ;
     POINTL              ptl ;
     SIZEF               sizef ;

     switch (msg)
	  {
          case WM_CREATE:
                         // Get the array of FONTMETRICS structures

               hps = WinGetPS (hwnd) ;
               GpiQueryFontAction (hps, QFA_PUBLIC) ;
               lFonts = GetAllOutlineFonts (hps, & pfm) ;

                         // Get the font resolution of the device

               hdc = GpiQueryDevice (hps) ;
               DevQueryCaps (hdc, CAPS_HORIZONTAL_FONT_RES, 1, &xRes) ;
               DevQueryCaps (hdc, CAPS_VERTICAL_FONT_RES,   1, &yRes) ;
               WinReleasePS (hps) ;

               return 0 ;

          case WM_SIZE:
               cyClient = HIUSHORT (mp2) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               GpiErase (hps) ;

                    // Get new fonts if they've changed

               if (QFA_PUBLIC & GpiQueryFontAction (hps, QFA_PUBLIC))
                    {
                    free (pfm) ;
                    lFonts = GetAllOutlineFonts (hps, & pfm) ;
                    }

                    // Set POINTL structure to upper left corner of client

               ptl.x = 0 ;
               ptl.y = cyClient ;

                    // Set the character box for the point size

               sizef.cx = 65536 * xRes * PTSIZE / 72 ;
               sizef.cy = 65536 * yRes * PTSIZE / 72 ;

               GpiSetCharBox (hps, &sizef) ;

                    // Loop through all the bitmap fonts

               for (l = 0 ; l < lFonts ; l++)
                    {
                         // Define the FATTRS structure

                    fat.usRecordLength  = sizeof (FATTRS) ;
                    fat.fsSelection     = 0 ;
                    fat.lMatch          = 0 ;

                    strcpy (fat.szFacename, pfm[l].szFacename) ;

                    fat.idRegistry      = pfm[l].idRegistry ;
                    fat.usCodePage      = pfm[l].usCodePage ;
                    fat.lMaxBaselineExt = 0 ;
                    fat.lAveCharWidth   = 0 ;
                    fat.fsType          = FATTR_FONTUSE_OUTLINE |
                                          FATTR_FONTUSE_TRANSFORMABLE ;
                    fat.fsFontUse       = 0 ;

                         // Create the logical font and select it

                    GpiCreateLogFont (hps, NULL, LCID_FONT, &fat) ;
                    GpiSetCharSet (hps, LCID_FONT) ;

                         // Query the font metrics of the current font

                    GpiQueryFontMetrics (hps, sizeof (FONTMETRICS), &fm) ;

                         // Set up a text string to display

                    sprintf (szBuffer, " %s - %d points",
                             fm.szFacename, PTSIZE) ;

                         // Drop POINTL structure to baseline of font

                    ptl.y -= fm.lMaxAscender ;

                         // Display the character string

                    GpiCharStringAt (hps, &ptl, strlen (szBuffer), szBuffer) ;

                         // Drop POINTL structure down to bottom of text

                    ptl.y -= fm.lMaxDescender ;

                         // Select the default font; delete the logical font

                    GpiSetCharSet (hps, LCID_DEFAULT) ;
                    GpiDeleteSetId (hps, LCID_FONT) ;
                    }

               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               if (lFonts > 0)
                    free (pfm) ;

               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
