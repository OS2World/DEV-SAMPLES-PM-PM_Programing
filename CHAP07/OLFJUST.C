/*------------------------------------------
   OLFJUST.C -- Justified OS/2 Outline Font
                (c) Charles Petzold, 1993
  ------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include "olf.h"

#define LCID_FONT        1
#define FACENAME         "Times New Roman"
#define PTWIDTH          200
#define PTHEIGHT         200

#define ALIGN_LEFT       1
#define ALIGN_RIGHT      2
#define ALIGN_CENTER     3
#define ALIGN_JUST       4

#define SPACE_SINGLE     1
#define SPACE_HALF       2
#define SPACE_DOUBLE     3

VOID Justify (HPS hps, PCHAR pText, PRECTL prcl, SHORT nAlign, SHORT nSpace)
     {
     int      iBreakCount, iSurplus ;
     PCHAR    pStart, pEnd ;
     POINTL   ptlStart, aptlTextBox [TXTBOX_COUNT] ;

     ptlStart.y = prcl->yTop ;

     do                                 // until end of text
          {
          iBreakCount = 0 ;

          while (*pText == ' ')         // Skip over leading blanks
               pText++ ;

          pStart = pText ;

          do                            // until line is known
               {
               while (*pText == ' ')    // Skip over leading blanks
                    pText++ ;

                                        // Find next break point

               while (*pText != '\x00' && *pText != ' ')
                    pText++ ;

                                        // Determine text width

               GpiQueryTextBox (hps, pText - pStart, pStart,
                                TXTBOX_COUNT, aptlTextBox) ;

                         // Normal case: text less wide than column

               if (aptlTextBox[TXTBOX_CONCAT].x < (prcl->xRight - prcl->xLeft))
                    {
                    iBreakCount++ ;
                    pEnd = pText ;
                    }

                         // Text wider than window with only one word

               else if (iBreakCount == 0)
                    {
                    pEnd = pText ;
                    break ;
                    }

                         // Text wider than window, so fix up and get out
               else
                    {
                    iBreakCount-- ;
                    pText = pEnd ;
                    break ;
                    }
               }
          while (*pText != '\x00') ;

                         // Get the final text box

          GpiQueryTextBox (hps, pEnd - pStart, pStart,
                           TXTBOX_COUNT, aptlTextBox) ;

                         // Drop down by maximum ascender

          ptlStart.y -= aptlTextBox[TXTBOX_TOPLEFT].y ;

                         // Find surplus space in text line

          iSurplus = prcl->xRight - prcl->xLeft -
                     aptlTextBox[TXTBOX_CONCAT].x ;

                        // Adjust starting position and
                        // space and character spacing

          switch (nAlign)
               {
               case ALIGN_LEFT:
                    ptlStart.x = prcl->xLeft ;
                    break ;

               case ALIGN_RIGHT:
                    ptlStart.x = prcl->xLeft + iSurplus ;
                    break ;

               case ALIGN_CENTER:
                    ptlStart.x = prcl->xLeft + iSurplus / 2 ;
                    break ;

               case ALIGN_JUST:
                    ptlStart.x = prcl->xLeft ;

                    if (*pText == '\x00')
                         break ;

                    if (iBreakCount > 0)
                         GpiSetCharBreakExtra (hps,
                              65536 * iSurplus /  iBreakCount) ;

                    else if (pEnd - pStart - 1 > 0)
                         GpiSetCharExtra (hps,
                              65536 * iSurplus / (pEnd - pStart - 1)) ;
                    break ;
               }

                         // Display the string & return to normal

          GpiCharStringAt (hps, &ptlStart, pEnd - pStart, pStart) ;
          GpiSetCharExtra (hps, 0) ;
          GpiSetCharBreakExtra (hps, 0) ;

                         // Drop down by maximum descender

          ptlStart.y += aptlTextBox[TXTBOX_BOTTOMLEFT].y ;

                         // Do additional line-spacing

          switch (nSpace)
               {
               case SPACE_HALF:
                    ptlStart.y -= (aptlTextBox[TXTBOX_TOPLEFT].y -
                                   aptlTextBox[TXTBOX_BOTTOMLEFT].y) / 2 ;
                    break ;

               case SPACE_DOUBLE:
                    ptlStart.y -= aptlTextBox[TXTBOX_TOPLEFT].y -
                                  aptlTextBox[TXTBOX_BOTTOMLEFT].y ;
                    break ;
               }
          }

     while (*pText != '\x00' && ptlStart.y > prcl->yBottom) ;
     }

void PaintClient (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     static CHAR szText [] =

               "You don't know about me, without you have read a book by "
               "the name of \"The Adventures of Tom Sawyer,\" but that "
               "ain't no matter. That book was made by Mr. Mark Twain, "
               "and he told the truth, mainly. There was things which he "
               "stretched, but mainly he told the truth. That is nothing. "
               "I never seen anybody but lied, one time or another, "
               "without it was Aunt Polly, or the widow, or maybe Mary. "
               "Aunt Polly - Tom's Aunt Polly, she is - and Mary, and the "
               "Widow Douglas, is all told about in that book - which is "
               "mostly a true book; with some stretchers, as I said before." ;

     RECTL       rcl ;

          // Create and size the logical font

     CreateOutlineFont (hps, LCID_FONT, FACENAME, 0, 0) ;
     GpiSetCharSet (hps, LCID_FONT) ;
     ScaleOutlineFont (hps, PTWIDTH, PTHEIGHT) ;

          // Display the text

     rcl.xLeft   = 0 ;
     rcl.yBottom = 0 ;
     rcl.xRight  = cxClient ;
     rcl.yTop    = cyClient ;

     Justify (hps, szText, &rcl, ALIGN_JUST, SPACE_SINGLE) ;

          // Select the default font; delete the logical font

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_FONT) ;
     }
