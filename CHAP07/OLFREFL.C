/*-------------------------------------------
   OLFREFL.C -- Reflected OS/2 Outline Fonts
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
     static CHAR szText [] = "Reflect" ;
     int         i ;
     POINTL      ptl ;

          // Set POINTL structure to center of client

     ptl.x = cxClient / 2 ;
     ptl.y = cyClient / 2 ;

          // Create the logical font and select it

     CreateOutlineFont (hps, LCID_FONT, "Times New Roman", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;

     for (i = 0 ; i < 4 ; i++)
          {
          ScaleOutlineFont (hps, (i > 1) ? -720 : 720,
                                 (i & 1) ? -720 : 720) ;

               // Display the character string

          GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;
          }

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
