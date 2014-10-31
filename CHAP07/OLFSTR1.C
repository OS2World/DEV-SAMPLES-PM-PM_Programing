/*-------------------------------------------
   OLFSTR1.C -- Stretched OS/2 Outline Fonts
               (c) Charles Petzold, 1993
  -------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     static CHAR szText [] = "Hello!" ;
     FONTMETRICS fm ;
     int         iPtHt, iPtWd ;
     POINTL      ptl, aptl[TXTBOX_COUNT] ;

          // Create the logical font, select it, and scale it

     CreateOutlineFont (hps, LCID_FONT, "Helvetica", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, 120, 120) ;

          // Scale font to client window size

     GpiQueryTextBox (hps, strlen (szText), szText, TXTBOX_COUNT, aptl) ;

     iPtHt = (int) (120 * cyClient / (aptl[TXTBOX_TOPLEFT].y -
                                      aptl[TXTBOX_BOTTOMLEFT].y)) ;
     iPtWd = (int) (120 * cxClient /  aptl[TXTBOX_CONCAT].x) ;

     ScaleOutlineFont (hps, iPtHt, iPtWd) ;

          // Display the text string

     GpiQueryFontMetrics (hps, sizeof (FONTMETRICS), &fm) ;

     ptl.x = 0 ;
     ptl.y = cyClient - fm.lMaxAscender ;

     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
