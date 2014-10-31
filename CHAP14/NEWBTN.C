/*--------------------------------------------------------------
   NEWBTN.C -- Contains window procedure for new 3D push button
               (c) Charles Petzold, 1993
  --------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <string.h>

#define LCID_ITALIC 1L

               /*--------------------------------------------------
                  Structure for storing data unique to each window
		 --------------------------------------------------*/
typedef struct
     {
     PSZ  pszText ;
     BOOL fHaveCapture ;
     BOOL fHaveFocus ;
     BOOL fInsideRect ;
     BOOL fSpaceDown ;
     }
     NEWBTN ;

typedef NEWBTN *PNEWBTN ;

MRESULT EXPENTRY NewBtnWndProc (HWND, ULONG, MPARAM, MPARAM) ;
VOID             DrawButton    (HWND, HPS, PNEWBTN) ;

          /*---------------------------------------------------------
             RegisterNewBtnClass function available to other modules
            ---------------------------------------------------------*/

BOOL APIENTRY RegisterNewBtnClass (HAB hab)
     {
     return WinRegisterClass (hab, "NewBtn", NewBtnWndProc,
                              CS_SIZEREDRAW, sizeof (PNEWBTN)) ;
     }

          /*--------------------------------
             NewBtnWndProc window procedure
            --------------------------------*/

MRESULT EXPENTRY NewBtnWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     BOOL          fTestInsideRect ;
     HPS           hps ;
     CREATESTRUCT  *pcrst ;
     POINTL        ptl ;
     NEWBTN        *pNewBtn ;
     WNDPARAMS     *pwprm ;
     RECTL         rcl ;

     pNewBtn = WinQueryWindowPtr (hwnd, 0) ;

     switch (msg)
          {
          case WM_CREATE:
               pNewBtn = malloc (sizeof (NEWBTN)) ;

                         // Initialize structure

               pNewBtn->fHaveCapture = FALSE ;
               pNewBtn->fHaveFocus   = FALSE ;
               pNewBtn->fInsideRect  = FALSE ;
               pNewBtn->fSpaceDown   = FALSE ;

                         // Get window text from creation structure

               pcrst = (PCREATESTRUCT) PVOIDFROMMP (mp2) ;

               pNewBtn->pszText = malloc (1 + strlen (pcrst->pszText)) ;
               strcpy (pNewBtn->pszText, pcrst->pszText) ;

               WinSetWindowPtr (hwnd, 0, pNewBtn) ;
               return 0 ;

          case WM_SETWINDOWPARAMS:
               pwprm = (PWNDPARAMS) PVOIDFROMMP (mp1) ;

                         // Get window text from window parameter structure

               if (pwprm->fsStatus & WPM_TEXT)
                    {
                    free (pNewBtn->pszText) ;
                    pNewBtn->pszText = malloc (1 + pwprm->cchText) ;
                    strcpy (pNewBtn->pszText, pwprm->pszText) ;
                    }
               return MRFROMSHORT (1) ;

          case WM_QUERYWINDOWPARAMS:
               pwprm = (PWNDPARAMS) PVOIDFROMMP (mp1) ;

                         // Set window parameter structure fields

               if (pwprm->fsStatus & WPM_CCHTEXT)
                    pwprm->cchText = strlen (pNewBtn->pszText) ;

               if (pwprm->fsStatus & WPM_TEXT)
                    strcpy (pwprm->pszText, pNewBtn->pszText) ;

               if (pwprm->fsStatus & WPM_CBPRESPARAMS)
                    pwprm->cbPresParams = 0 ;

               if (pwprm->fsStatus & WPM_PRESPARAMS)
                    pwprm->pPresParams = NULL ;

               if (pwprm->fsStatus & WPM_CBCTLDATA)
                    pwprm->cbCtlData = 0 ;

               if (pwprm->fsStatus & WPM_CTLDATA)
                    pwprm->pCtlData = NULL ;

               return MRFROMSHORT (1) ;

          case WM_BUTTON1DOWN:
               WinSetFocus (HWND_DESKTOP, hwnd) ;
               WinSetCapture (HWND_DESKTOP, hwnd) ;
               pNewBtn->fHaveCapture = TRUE ;
               pNewBtn->fInsideRect  = TRUE ;
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_MOUSEMOVE:
               if (!pNewBtn->fHaveCapture)
                    break ;

               WinQueryWindowRect (hwnd, &rcl) ;
               ptl.x = MOUSEMSG(&msg)->x ;
               ptl.y = MOUSEMSG(&msg)->y ;

                         // Test if mouse pointer is still in window

               fTestInsideRect = WinPtInRect (WinQueryAnchorBlock (hwnd),
                                              &rcl, &ptl) ;

               if (pNewBtn->fInsideRect != fTestInsideRect)
                    {
                    pNewBtn->fInsideRect = fTestInsideRect ;
                    WinInvalidateRect (hwnd, NULL, FALSE) ;
                    }
               break ;

          case WM_BUTTON1UP:
               if (!pNewBtn->fHaveCapture)
                    break ;

               WinSetCapture (HWND_DESKTOP, NULLHANDLE) ;
               pNewBtn->fHaveCapture = FALSE ;
               pNewBtn->fInsideRect  = FALSE ;

               WinQueryWindowRect (hwnd, &rcl) ;
               ptl.x = MOUSEMSG(&msg)->x ;
               ptl.y = MOUSEMSG(&msg)->y ;

                         // Post WM_COMMAND if mouse pointer is in window

               if (WinPtInRect (WinQueryAnchorBlock (hwnd), &rcl, &ptl))
                    WinPostMsg (WinQueryWindow (hwnd, QW_OWNER),
                         WM_COMMAND,
                         MPFROMSHORT (WinQueryWindowUShort (hwnd, QWS_ID)),
                         MPFROM2SHORT (CMDSRC_OTHER, TRUE)) ;

               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_ENABLE:
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_SETFOCUS:
               pNewBtn->fHaveFocus = SHORT1FROMMP (mp2) ;
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_CHAR:
               if (!(CHARMSG(&msg)->fs   &  KC_VIRTUALKEY) ||
                     CHARMSG(&msg)->vkey != VK_SPACE       ||
                     CHARMSG(&msg)->fs   &  KC_PREVDOWN)
                    break ;

                         // Post WM_COMMAND when space bar is released

               if (!(CHARMSG(&msg)->fs & KC_KEYUP))
                    pNewBtn->fSpaceDown = TRUE ;
               else
                    {
                    pNewBtn->fSpaceDown = FALSE ;
                    WinPostMsg (WinQueryWindow (hwnd, QW_OWNER),
                         WM_COMMAND,
                         MPFROMSHORT (WinQueryWindowUShort (hwnd, QWS_ID)),
                         MPFROM2SHORT (CMDSRC_OTHER, FALSE)) ;
                    }
               WinInvalidateRect (hwnd, NULL, FALSE) ;
               return 0 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;
               DrawButton (hwnd, hps, pNewBtn) ;
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               free (pNewBtn->pszText) ;
               free (pNewBtn) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

          /*--------------------------------------------------------
             Draws filled and outlined polygon (used by DrawButton)
            --------------------------------------------------------*/

VOID Polygon (HPS hps, LONG lPoints, POINTL aptl[], LONG lColor)
     {
               // Draw interior in specified color

     GpiSavePS (hps) ;
     GpiSetColor (hps, lColor) ;

     GpiBeginArea (hps, BA_NOBOUNDARY | BA_ALTERNATE) ;
     GpiMove (hps, aptl) ;
     GpiPolyLine (hps, lPoints - 1, aptl + 1) ;
     GpiEndArea (hps) ;

     GpiRestorePS (hps, -1L) ;

               // Draw boundary in default color

     GpiMove (hps, aptl + lPoints - 1) ;
     GpiPolyLine (hps, lPoints, aptl) ;
     }

          /*---------------------
             Draws Square Button
            ---------------------*/

VOID DrawButton (HWND hwnd, HPS hps, PNEWBTN pNewBtn)
     {
     FATTRS      fat ;
     FONTMETRICS fm ;
     HDC         hdc ;
     LONG        lColor, lHorzRes, lVertRes, cxEdge, cyEdge ;
     POINTL      aptl[10], aptlTextBox[TXTBOX_COUNT], ptlShadow, ptlText ;
     RECTL       rcl ;

               // Find 2 millimeter edge width in pixels

     hdc = GpiQueryDevice (hps) ;
     DevQueryCaps (hdc, CAPS_HORIZONTAL_RESOLUTION, 1L, &lHorzRes) ;
     DevQueryCaps (hdc, CAPS_VERTICAL_RESOLUTION,   1L, &lVertRes) ;

     cxEdge = lHorzRes / 500 ;
     cyEdge = lVertRes / 500 ;

               // Set up coordinates for drawing the button

     WinQueryWindowRect (hwnd, &rcl) ;

     aptl[0].x = 0 ;                    aptl[0].y = 0 ;
     aptl[1].x = cxEdge ;               aptl[1].y = cyEdge ;
     aptl[2].x = rcl.xRight - cxEdge ;  aptl[2].y = cyEdge ;
     aptl[3].x = rcl.xRight - 1 ;       aptl[3].y = 0 ;
     aptl[4].x = rcl.xRight - 1 ;       aptl[4].y = rcl.yTop - 1 ;
     aptl[5].x = rcl.xRight - cxEdge ;  aptl[5].y = rcl.yTop - cyEdge ;
     aptl[6].x = cxEdge ;               aptl[6].y = rcl.yTop - cyEdge ;
     aptl[7].x = 0 ;                    aptl[7].y = rcl.yTop - 1 ;
     aptl[8].x = 0 ;                    aptl[8].y = 0 ;
     aptl[9].x = cxEdge ;               aptl[9].y = cyEdge ;

               // Paint edges at bottom and right side

     GpiSetColor (hps, CLR_BLACK) ;
     lColor = (pNewBtn->fInsideRect || pNewBtn->fSpaceDown) ?
                              CLR_PALEGRAY : CLR_DARKGRAY ;
     Polygon (hps, 4L, aptl + 0, lColor) ;
     Polygon (hps, 4L, aptl + 2, lColor) ;

               // Paint edges at top and left side

     lColor = (pNewBtn->fInsideRect || pNewBtn->fSpaceDown) ?
                              CLR_DARKGRAY : CLR_WHITE ;
     Polygon (hps, 4L, aptl + 4, lColor) ;
     Polygon (hps, 4L, aptl + 6, lColor) ;

               // Paint interior area

     GpiSavePS (hps) ;
     GpiSetColor (hps, (pNewBtn->fInsideRect || pNewBtn->fSpaceDown) ?
                              CLR_DARKGRAY : CLR_PALEGRAY) ;
     GpiMove (hps, aptl + 1) ;
     GpiBox (hps, DRO_FILL, aptl + 5, 0L, 0L) ;
     GpiRestorePS (hps, -1L) ;
     GpiBox (hps, DRO_OUTLINE, aptl + 5, 0L, 0L) ;

               // If button has focus, use italic font

     GpiQueryFontMetrics (hps, (LONG) sizeof fm, &fm) ;

     if (pNewBtn->fHaveFocus)
          {
          fat.usRecordLength  = sizeof fat ;
          fat.fsSelection     = FATTR_SEL_ITALIC ;
          fat.lMatch          = 0 ;
          fat.idRegistry      = fm.idRegistry ;
          fat.usCodePage      = fm.usCodePage ;
          fat.lMaxBaselineExt = fm.lMaxBaselineExt ;
          fat.lAveCharWidth   = fm.lAveCharWidth ;
          fat.fsType          = 0 ;
          fat.fsFontUse       = 0 ;
          strcpy (fat.szFacename, fm.szFacename) ;

          GpiCreateLogFont (hps, NULL, LCID_ITALIC, &fat) ;
          GpiSetCharSet (hps, LCID_ITALIC) ;
          }
               // Calculate text position

     GpiQueryTextBox (hps, (LONG) strlen (pNewBtn->pszText), pNewBtn->pszText,
                           TXTBOX_COUNT, aptlTextBox) ;

     ptlText.x = (rcl.xRight - aptlTextBox[TXTBOX_CONCAT].x) / 2 ;
     ptlText.y = (rcl.yTop   - aptlTextBox[TXTBOX_TOPLEFT].y -
                               aptlTextBox[TXTBOX_BOTTOMLEFT].y) / 2 ;

     ptlShadow.x = ptlText.x + fm.lAveCharWidth   / 3 ;
     ptlShadow.y = ptlText.y - fm.lMaxBaselineExt / 8 ;

               // Display text shadow in black, and text in white

     GpiSetColor (hps, CLR_BLACK) ;
     GpiCharStringAt (hps, &ptlShadow, (LONG) strlen (pNewBtn->pszText),
                                       pNewBtn->pszText) ;
     GpiSetColor (hps, CLR_WHITE) ;
     GpiCharStringAt (hps, &ptlText, (LONG) strlen (pNewBtn->pszText),
                                     pNewBtn->pszText) ;

               // X out button if the window is not enabled

     if (!WinIsWindowEnabled (hwnd))
          {
          GpiMove (hps, aptl + 1) ;
          GpiLine (hps, aptl + 5) ;
          GpiMove (hps, aptl + 2) ;
          GpiLine (hps, aptl + 6) ;
          }
               // Clean up

     if (pNewBtn->fHaveFocus)
          {
          GpiSetCharSet (hps, LCID_DEFAULT) ;
          GpiDeleteSetId (hps, LCID_ITALIC) ;
          }
     }
