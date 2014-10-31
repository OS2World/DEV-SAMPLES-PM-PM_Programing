/*----------------------------------------
   OLFLIST.C -- Lists OS/2 Outline Fonts
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "olf.h"

#define LCID_FONT     1
#define PTSIZE      120
#define PTWIDTH     120

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     CHAR         szBuffer [FACESIZE + 32] ;
     FONTMETRICS  fm ;
     int          i ;
     PFACELIST    pfl ;
     POINTL       ptl ;

          // Get pointer to FACELIST structure

     pfl = GetAllOutlineFonts (hps) ;

          // Set POINTL structure to upper left corner of client

     ptl.x = 0 ;
     ptl.y = cyClient ;

          // Loop through all the outline fonts

     for (i = 0 ; i < pfl->iNumFaces ; i++)
          {
               // Create the logical font and select it

          CreateOutlineFont (hps, LCID_FONT, pfl->szFacename[i], 0, 0) ;
          GpiSetCharSet (hps, LCID_FONT) ;

               // Scale the selected font

          ScaleOutlineFont (hps, PTSIZE, PTWIDTH) ;

               // Query the font metrics of the current font

          GpiQueryFontMetrics (hps, sizeof (FONTMETRICS), &fm) ;

               // Set up a text string to display

          sprintf (szBuffer, "%s - %d decipoints, width %d",
                   fm.szFacename, PTSIZE, PTWIDTH) ;

               // Drop POINTL structure to baseline of font

          ptl.y -= fm.lMaxAscender ;

               // Display the character string

          GpiCharStringAt (hps, &ptl, strlen (szBuffer), szBuffer) ;

               // Drop POINTL structure down to bottom of text

          ptl.y -= fm.lMaxDescender ;

               // Select the default font; delete the logical font

          GpiSetCharSet (hps, LCID_DEFAULT) ;
          GpiDeleteSetId (hps, LCID_FONT) ;

          if (ptl.y < 0)
               break ;
          }
     }
