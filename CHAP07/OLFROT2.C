/*-----------------------------------------------------
   OLFROT2.C -- Rotated and Sheared OS/2 Outline Fonts
                (c) Charles Petzold, 1993
  -----------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <math.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT    1
#define TWO_PI       (2 * 3.14159)

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     static CHAR szText [] = "  Rotated Font" ;
     double      dAngle ;
     GRADIENTL   gradl ;
     POINTL      ptl, ptlShear ;

          // Set POINTL structure to center of client

     ptl.x = cxClient / 2 ;
     ptl.y = cyClient / 2 ;

          // Create the logical font, select it, and scale it

     CreateOutlineFont (hps, LCID_FONT, "Helvetica", 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, 240, 240) ;

          // Loop through the character angles

     for (dAngle = 11.25 ; dAngle < 360 ; dAngle += 22.5)
          {
               // Set the character angle

          gradl.x = (LONG) (100 * cos (TWO_PI * dAngle / 360)) ;
          gradl.y = (LONG) (100 * sin (TWO_PI * dAngle / 360)) ;

          GpiSetCharAngle (hps, &gradl) ;

               // Set the character shear

          ptlShear.x = (LONG) (100 * cos (TWO_PI * (90 - dAngle) / 360)) ;
          ptlShear.y = (LONG) (100 * sin (TWO_PI * (90 - dAngle) / 360)) ;

          GpiSetCharShear (hps, &ptlShear) ;

               // Display the character string

          GpiCharStringAt (hps, &ptl, strlen (szText), szText) ;
          }

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
