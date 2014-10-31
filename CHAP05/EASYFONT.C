/*----------------------------------------------
   EASYFONT.C -- Routines for Using Image Fonts
                 (c) Charles Petzold, 1993
  ----------------------------------------------*/

#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include "easyfont.h"

static LONG  alMatch[5][6] ;
static PCHAR szFacename[5] = { "System Proportional",
                               "System Monospaced",
                               "Courier", "Helv", "Tms Rmn" } ;
static INT   iFontSize [6] = { 80, 100, 120, 140, 180, 240 } ;

BOOL EzfQueryFonts (HPS hps)
     {
     PFONTMETRICS pfm ;
     HDC          hdc ;
     INT          iIndex, iFace, iSize ;
     LONG         lHorzRes, lVertRes, lRequestFonts, lNumberFonts ;

     hdc = GpiQueryDevice (hps) ;
     DevQueryCaps (hdc, CAPS_HORIZONTAL_FONT_RES, 1L, &lHorzRes) ;
     DevQueryCaps (hdc, CAPS_VERTICAL_FONT_RES,   1L, &lVertRes) ;

     for (iFace = 0 ; iFace < 5 ; iFace++)
          {
          lRequestFonts = 0 ;
          lNumberFonts = GpiQueryFonts (hps, QF_PUBLIC, szFacename[iFace],
                                        &lRequestFonts, 0L, NULL) ;
          if (lNumberFonts == 0)
               continue ;

          pfm = malloc (lNumberFonts * sizeof (FONTMETRICS)) ;

          if (pfm == NULL)
               return FALSE ;

          GpiQueryFonts (hps, QF_PUBLIC, szFacename[iFace],
                         &lNumberFonts, sizeof (FONTMETRICS), pfm) ;

          for (iIndex = 0 ; iIndex < lNumberFonts ; iIndex++)
               if (pfm[iIndex].sXDeviceRes == (SHORT) lHorzRes &&
                   pfm[iIndex].sYDeviceRes == (SHORT) lVertRes &&
                  (pfm[iIndex].fsDefn & 1) == 0)
                    {
                    for (iSize = 0 ; iSize < 6 ; iSize++)
                         if (pfm[iIndex].sNominalPointSize == iFontSize[iSize])
                              break ;

                    if (iSize != 6)
                         alMatch[iFace][iSize] = pfm[iIndex].lMatch ;
                    }

          free (pfm) ;
          }
     return TRUE ;
     }

LONG EzfCreateLogFont (HPS hps, LONG lcid, INT idFace, INT idSize,
                                           USHORT fsSelection)
     {
     static FATTRS fat ;

     if (idFace > 4 || idSize > 5 || alMatch[idFace][idSize] == 0)
          return FALSE ;

     fat.usRecordLength = sizeof fat ;
     fat.fsSelection    = fsSelection ;
     fat.lMatch         = alMatch[idFace][idSize] ;

     strcpy (fat.szFacename, szFacename[idFace]) ;

     return GpiCreateLogFont (hps, NULL, lcid, &fat) ;
     }
