/*----------------------------------------
   OLFSIZE.C -- OS/2 Outline Fonts Sizes
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1
#define FACENAME     "Times New Roman Italic"

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     CHAR        szBuffer [FACESIZE + 32] ;
     FONTMETRICS fm ;
     int         i ;
     POINTL      aptlTextBox [TXTBOX_COUNT], ptl ;

          // Set POINTL structure to upper left corner of client

     ptl.x = 0 ;
     ptl.y = cyClient ;

          // Create the logical font and select it

     CreateOutlineFont (hps, LCID_FONT, FACENAME, 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;

          // Loop through the font sizes

     i = 100 ;

     while (ptl.y > 0)
          {
               // Scale the selected font

          ScaleOutlineFont (hps, i, i) ;

               // Query the font metrics of the current font

          GpiQueryFontMetrics (hps, sizeof (FONTMETRICS), &fm) ;

               // Set up a text string to display

          sprintf (szBuffer, "%s - %d decipoints", fm.szFacename, i) ;

          GpiQueryTextBox (hps, strlen (szBuffer), szBuffer,
                           TXTBOX_COUNT, aptlTextBox) ;

          sprintf (szBuffer, "%s - %d decipoints (%d x %d)", fm.szFacename, i,
                   aptlTextBox[TXTBOX_CONCAT].x,
                   aptlTextBox[TXTBOX_TOPLEFT].y -
                   aptlTextBox[TXTBOX_BOTTOMLEFT].y) ;

               // Drop POINTL structure to baseline of font

          ptl.y -= fm.lMaxAscender ;

               // Display the character string

          GpiCharStringAt (hps, &ptl, strlen (szBuffer), szBuffer) ;

               // Drop POINTL structure down to bottom of text

          ptl.y -= fm.lMaxDescender ;

          i++ ;
          }

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
