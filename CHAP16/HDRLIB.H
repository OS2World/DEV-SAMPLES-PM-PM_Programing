/*--------------------------------------------------
   HDRLIB.H -- "Handy Drawing Routines" Header File
  --------------------------------------------------*/

INT  APIENTRY  HdrPuts    (HPS hps, PPOINTL pptl, PCHAR szText) ;
INT  APIENTRY  HdrPrintf  (HPS hps, PPOINTL pptl, PCHAR szFormat, ...) ;
LONG APIENTRY  HdrEllipse (HPS hps, LONG lOption, PPOINTL pptl) ;
