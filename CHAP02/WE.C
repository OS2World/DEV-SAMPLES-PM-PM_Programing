/*-------------------------------------------------------
   WE.C -- A Program that Obtains an Anchor Block Handle
           (c) Charles Petzold, 1993
  -------------------------------------------------------*/

#include <os2.h>

int main (void)
     {
     HAB  hab ;

     hab = WinInitialize (0) ;

     WinTerminate (hab) ;
     return 0 ;
     }
