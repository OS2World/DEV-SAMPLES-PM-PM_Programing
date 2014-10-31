/*-----------------------------------------
   OLFSHAD.C -- Shadowed OS/2 Outline Font
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
     static CHAR szText [] = "Shadow" ;
     POINTL      ptl, ptlShear ;

          // Color the client window

     GpiSetColor (hps, CLR_BLUE) ;

     ptl.x = 0 ;
     ptl.y = 0 ;
     GpiMove (hps, &ptl) ;

     ptl.x = cxClient ;
     ptl.y = cyClient ;
     GpiBox (hps, DRO_FILL, &ptl, 0, 0) ;

          // Create the logical font

     CreateOutlineFont (hps, LCID_FONT, "Times New Roman", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;

          // Display the shadow

     GpiSetColor (hps, CLR_DARKBLUE) ;

     ScaleOutlineFont (hps, 2160, 720) ;

     ptlShear.x = 2 ;
     ptlShear.y = 1 ;
     GpiSetCharShear (hps, &ptlShear) ;

     ptl.x = cxClient / 8 ;
     ptl.y = cyClient / 4 ;
     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

          // Display the text

     GpiSetColor (hps, CLR_RED) ;

     ScaleOutlineFont (hps, 720, 720) ;

     ptlShear.x = 0 ;
     ptlShear.y = 1 ;
     GpiSetCharShear (hps, &ptlShear) ;

     GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
