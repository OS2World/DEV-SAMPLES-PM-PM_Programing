/*--------------------------------------------
   OLF.C -- Easy access to OS/2 outline fonts
            (c) Charles Petzold, 1993
  --------------------------------------------*/

#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include "olf.h"

PFACELIST GetAllOutlineFonts (HPS hps)
     {
     static PFACELIST pfl ;
     LONG             l, lFonts ;
     PFONTMETRICS     pfm ;

               // Check for changed fonts

     if (!(QFA_PUBLIC & GpiQueryFontAction (hps, QFA_PUBLIC)) && pfl != NULL)
          return pfl ;

               // Delete old structure if necessary

     if (pfl != NULL)
          free (pfl) ;

               // Determine the number of fonts

     lFonts = 0 ;
     lFonts = GpiQueryFonts (hps, QF_PUBLIC, NULL, &lFonts, 0, NULL) ;

     if (lFonts == 0)
          return NULL ;

               // Allocate memory for FONTMETRICS structures

     pfm = (PFONTMETRICS) calloc (lFonts, sizeof (FONTMETRICS)) ;

     if (pfm == NULL)
          return NULL ;

               // Get all fonts

     GpiQueryFonts (hps, QF_PUBLIC, NULL, &lFonts,
                         sizeof (FONTMETRICS), pfm) ;

               // Allocate memory for FACELIST structure

     pfl = malloc (sizeof (FACELIST)) ;
     pfl->iNumFaces = 0 ;

               // Loop through all fonts

     for (l = 0 ; l < lFonts ; l++)
          {
                    // Check if outline font

          if (pfm[l].fsDefn & FM_DEFN_OUTLINE)
               {
                         // Reallocate FACELIST structure & store face name

               pfl = realloc (pfl, sizeof (FACELIST) +
                                   pfl->iNumFaces * FACESIZE) ;

               strcpy (pfl->szFacename[pfl->iNumFaces], pfm[l].szFacename) ;

               pfl->iNumFaces ++ ;
               }
          }

               // Clean up

     free (pfm) ;

     return pfl ;
     }

LONG CreateOutlineFont (HPS hps, LONG lcid, char * szFacename,
                        SHORT fsAttributes, SHORT usCodePage)
     {
     FATTRS fat ;
     LONG   lReturn ;

               // Set up FATTRS structure

     fat.usRecordLength  = sizeof (FATTRS) ;
     fat.fsSelection     = fsAttributes ;
     fat.lMatch          = 0 ;
     fat.idRegistry      = 0 ;
     fat.usCodePage      = usCodePage ;
     fat.lMaxBaselineExt = 0 ;
     fat.lAveCharWidth   = 0 ;
     fat.fsType          = FATTR_FONTUSE_OUTLINE |
                           FATTR_FONTUSE_TRANSFORMABLE ;
     fat.fsFontUse       = 0 ;

     strcpy (fat.szFacename, szFacename) ;

               // Create the font

     lReturn = GpiCreateLogFont (hps, NULL, lcid, &fat) ;

               // If no match, try a symbol code page

     if (lReturn == FONT_DEFAULT && usCodePage == 0)
          {
          fat.usCodePage = 65400 ;

          lReturn = GpiCreateLogFont (hps, NULL, lcid, &fat) ;
          }

     return lReturn ;
     }

BOOL ScaleOutlineFont (HPS hps, int iPointSize, int iPointWidth)
     {
     HDC    hdc ;
     LONG   xRes, yRes ;
     POINTL aptl[2] ;
     SIZEF  sizef ;

               // Get font resolution in pixels per inch

     hdc = GpiQueryDevice (hps) ;

     DevQueryCaps (hdc, CAPS_HORIZONTAL_FONT_RES, 1, &xRes) ;
     DevQueryCaps (hdc, CAPS_VERTICAL_FONT_RES,   1, &yRes) ;

               // Find desired font size in pixels

     if (iPointWidth == 0)
          iPointWidth = iPointSize ;

     aptl[0].x = 0 ;
     aptl[0].y = 0 ;
     aptl[1].x = (16 * xRes * iPointWidth + 360) / 720 ;
     aptl[1].y = (16 * yRes * iPointSize  + 360) / 720 ;

               // Convert to page coordinates

     GpiConvert (hps, CVTC_DEVICE, CVTC_PAGE, 2L, aptl) ;

               // Set the character box

     sizef.cx = (aptl[1].x - aptl[0].x) << 12 ;
     sizef.cy = (aptl[1].y - aptl[0].y) << 12 ;

     return GpiSetCharBox (hps, &sizef) ;
     }
