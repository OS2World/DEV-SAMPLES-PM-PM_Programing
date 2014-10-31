/*--------------------------------------------
   OLFDROP.C -- Drop Shadow OS/2 Outline Font
                (c) Charles Petzold, 1993
  --------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     static CHAR szText [] = "Hello!" ;
     POINTL      ptl, aptlTextBox [TXTBOX_COUNT] ;

          // Create and size the logical font

     CreateOutlineFont (hps, LCID_FONT, "Times New Roman Italic", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, 1440, 1440) ;

          // Get the text box

     GpiQueryTextBox (hps, strlen (szText), szText,
                      TXTBOX_COUNT, aptlTextBox) ;

          // Display the font normally twice

     ptl.x = (cxClient - aptlTextBox [TXTBOX_CONCAT].x) / 2 ;
     ptl.y = (cyClient - aptlTextBox [TXTBOX_TOPLEFT].y
                       - aptlTextBox [TXTBOX_BOTTOMLEFT].y) / 2 ;

     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

     ptl.x -= 10 ;
     ptl.y += 10 ;

     GpiSetColor (hps, CLR_BACKGROUND) ;
     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

          // Create the path

     GpiBeginPath (hps, 1) ;
     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;
     GpiEndPath (hps) ;

          // Outline the path

     GpiSetColor (hps, CLR_NEUTRAL) ;
     GpiSetLineWidth (hps, LINEWIDTH_THICK) ;
     GpiOutlinePath (hps, 1, 0) ;

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
