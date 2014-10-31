/*-----------------------------------------
   OLFCLIP.C -- OS/2 Outline Font Clipping
                (c) Charles Petzold, 1993
  -----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     static CHAR szText [] = "Hello!" ;
     int         i ;
     POINTL      ptl, aptl[3], aptlTextBox [TXTBOX_COUNT] ;

          // Create and size the logical font

     CreateOutlineFont (hps, LCID_FONT, "Times New Roman Italic", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, 1440, 1440) ;

          // Get the text box

     GpiQueryTextBox (hps, strlen (szText), szText,
                      TXTBOX_COUNT, aptlTextBox) ;

          // Create the path

     GpiBeginPath (hps, 1) ;

     ptl.x = (cxClient - aptlTextBox [TXTBOX_CONCAT].x) / 2 ;
     ptl.y = (cyClient - aptlTextBox [TXTBOX_TOPLEFT].y
                       - aptlTextBox [TXTBOX_BOTTOMLEFT].y) / 2 ;

     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

     GpiEndPath (hps) ;

          // Set the clipping path

     GpiSetClipPath (hps, 1, SCP_AND | SCP_ALTERNATE) ;

          // Draw Bezier splines

     for (i = 0 ; i < cyClient ; i++)
          {
          GpiSetColor (hps, (i / 16) % 6 + 1) ;

          ptl.x = 0 ;
          ptl.y = i ;
          GpiMove (hps, &ptl) ;

          aptl[0].x = cxClient / 3 ;
          aptl[0].y = i + cyClient / 3 ;

          aptl[1].x = 2 * cxClient / 3 ;
          aptl[1].y = i - cyClient / 3 ;

          aptl[2].x = cxClient ;
          aptl[2].y = i ;

          GpiPolySpline (hps, 3, aptl) ;
          }

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
