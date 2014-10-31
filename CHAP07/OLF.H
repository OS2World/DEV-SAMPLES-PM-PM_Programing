/*--------------------------------------------
   OLF.H -- Easy access to OS/2 outline fonts
            (c) Charles Petzold, 1993
  --------------------------------------------*/

typedef struct
     {
     int  iNumFaces ;
     char szFacename [1] [FACESIZE] ;
     }
     FACELIST ;

typedef FACELIST * PFACELIST ;

PFACELIST GetAllOutlineFonts (HPS hps) ;

LONG CreateOutlineFont (HPS hps, LONG lcid, char * szFacename,
                        SHORT fsAttributes, SHORT usCodePage) ;

BOOL ScaleOutlineFont (HPS hps, int iPointSize, int iPointWidth) ;
