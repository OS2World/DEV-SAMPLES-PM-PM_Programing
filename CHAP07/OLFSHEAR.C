/*------------------------------------------
   OLFSHEAR.C -- Sheared OS/2 Outline Fonts
                 (c) Charles Petzold, 1993
  ------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1
#define TWO_PI       (2 * 3.14159)

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     CHAR        szBuffer [32] ;
     double      dAngle ;
     FONTMETRICS fm ;
     POINTL      ptl, ptlShear ;

          // Set POINTL structure to near-left top of client

     ptl.x = cxClient / 8 ;
     ptl.y = cyClient ;

          // Create the logical font, select it, and scale it

     CreateOutlineFont (hps, LCID_FONT, "Helvetica", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, 160, 160) ;

     GpiQueryFontMetrics (hps, sizeof (FONTMETRICS), &fm) ;

          // Loop through the shear angles

     for (dAngle = 0 ; dAngle <= 360 ; dAngle += 22.5)
          {
               // Set the shear angle

          ptlShear.x = (LONG) (100 * cos (TWO_PI * dAngle / 360)) ;
          ptlShear.y = (LONG) (100 * sin (TWO_PI * dAngle / 360)) ;

          GpiSetCharShear (hps, &ptlShear) ;

               // Display the character string

          ptl.y -= fm.lMaxAscender ;

          GpiCharStringAt (hps, &ptl,
               sprintf (szBuffer, "Character Shear (%.1f) degrees", dAngle),
                    szBuffer) ;

          ptl.y -= fm.lMaxDescender ;
          }

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
