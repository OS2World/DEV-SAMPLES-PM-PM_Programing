/*-----------------------------------------
   PRINTCAL.C -- Print a calendar
                 (c) Charles Petzold, 1993
  -----------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DEV
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include "printcal.h"

#define LCID_CALFONT          1L
#define STACKSIZE             8192
#define WM_USER_PRINT_OK      (WM_USER + 0)
#define WM_USER_PRINT_ERROR   (WM_USER + 1)

typedef struct
     {
     SHORT iYear, iMonthBeg, iMonthEnd ;
     }
     CALPARAMS ;

typedef struct
     {
     CALPARAMS cp ;
     HWND      hwndNotify ;
     }
     THREADPARAMS ;

MPARAM EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
MPARAM EXPENTRY AboutDlgProc  (HWND, ULONG, MPARAM, MPARAM) ;
MPARAM EXPENTRY PrintDlgProc  (HWND, ULONG, MPARAM, MPARAM) ;
VOID            DisplayPage   (HPS, SIZEL *, INT, INT) ;
VOID            Message       (HWND, INT, CHAR *) ;
VOID            PrintThread   (VOID *) ;
HDC             OpenDefaultPrinterDC (HAB) ;           // in PRINTDC.C

UINT uiActiveThreads = 0 ;

int main (void)
     {
     static CHAR  szClientClass[] = "PrintCal" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_MENU          |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;
     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, 0L, ID_RESOURCE, &hwndClient) ;
     while (TRUE)
          {
          while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
               WinDispatchMsg (hab, &qmsg) ;

          if (uiActiveThreads == 0)
               break ;

          Message (hwndClient, MB_ICONEXCLAMATION,
                   "Printing thread still active.\n"
                   "Program cannot be closed now.") ;

          WinCancelShutdown (hmq, FALSE) ;
          }

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MPARAM EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CALPARAMS cp ;
     static HAB       hab ;
     static HPS       hps ;
     static SIZEL     sizlClient ;
     DATETIME         dt ;
     HDC              hdc ;
     INT              iResult ;
     SIZEL            sizlPage ;
     THREADPARAMS     *ptp ;

     switch (msg)
          {
          case WM_CREATE:
               hab = WinQueryAnchorBlock (hwnd) ;
               hdc = WinOpenWindowDC (hwnd) ;
               sizlPage.cx = 0 ;
               sizlPage.cy = 0 ;
               hps = GpiCreatePS (hab, hdc, &sizlPage,
                                  PU_ARBITRARY | GPIF_DEFAULT |
                                  GPIT_MICRO   | GPIA_ASSOC) ;

               DosGetDateTime (&dt) ;
               cp.iYear     = dt.year ;
               cp.iMonthBeg = dt.month - 1 ;
               cp.iMonthEnd = dt.month - 1 ;
               return 0 ;

          case WM_SIZE:
               sizlClient.cx = SHORT1FROMMP (mp2) ;
               sizlClient.cy = SHORT2FROMMP (mp2) ;

               GpiConvert (hps, CVTC_DEVICE, CVTC_PAGE, 1L,
                           (PPOINTL) &sizlClient) ;
               return 0 ;

          case WM_PAINT:
               WinBeginPaint (hwnd, hps, NULL) ;

               GpiErase (hps) ;
               DisplayPage (hps, &sizlClient, cp.iYear, cp.iMonthBeg) ;

               WinEndPaint (hps) ;
               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case IDM_ABOUT:
                         WinDlgBox (HWND_DESKTOP, hwnd, AboutDlgProc,
                                    0, IDD_ABOUT, NULL) ;
                         return 0 ;

                    case IDM_PRINT:
                         iResult = WinDlgBox (HWND_DESKTOP, hwnd, PrintDlgProc,
                                              0, IDD_PRINT, &cp) ;

                         if (iResult == DID_CANCEL)
                              return 0 ;

                         WinInvalidateRect (hwnd, NULL, FALSE) ;

                         if (iResult == IDD_PREVIEW)
                              return 0 ;

                         if ((ptp = malloc (sizeof (THREADPARAMS))) == NULL)
                              {
                              Message (hwnd, MB_ICONEXCLAMATION,
                                  "Cannot allocate memory for print thread!") ;
                              return 0 ;
                              }

                         ptp->cp         = cp ;
                         ptp->hwndNotify = hwnd ;

                         if (-1 == _beginthread (PrintThread,
#ifdef __IBMC__
                                                 NULL,
#endif
                                                 STACKSIZE, ptp))
                              {
                              free (ptp) ;
                              Message (hwnd, MB_ICONEXCLAMATION,
                                       "Cannot create print thread!") ;
                              return 0 ;
                              }

                         uiActiveThreads++ ;
                         Message (hwnd, MB_ICONASTERISK,
                                  "Print job successfully started.") ;
                         return 0 ;
                    }
               break ;

          case WM_USER_PRINT_OK:
               ptp = PVOIDFROMMP (mp1) ;
               free (ptp) ;
               uiActiveThreads-- ;
               Message (hwnd, MB_ICONASTERISK,
                        "Print job sent to spooler.") ;
               return 0 ;

          case WM_USER_PRINT_ERROR:
               ptp = PVOIDFROMMP (mp1) ;
               free (ptp) ;
               uiActiveThreads-- ;
               Message (hwnd, MB_ICONEXCLAMATION,
                        "Error encountered during printing.") ;
               return 0 ;

          case WM_DESTROY:
               GpiDestroyPS (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

MRESULT EXPENTRY AboutDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     switch (msg)
          {
          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case DID_OK:
                    case DID_CANCEL:
                         WinDismissDlg (hwnd, TRUE) ;
                         return 0 ;
                    }
               break ;
          }
     return WinDefDlgProc (hwnd, msg, mp1, mp2) ;
     }

MRESULT EXPENTRY PrintDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CALPARAMS cpLocal, *pcpCurrent ;

     switch (msg)
          {
          case WM_INITDLG:
               pcpCurrent = PVOIDFROMMP (mp2) ;
               cpLocal = *pcpCurrent ;

               WinSendDlgItemMsg (hwnd, IDD_MONTHBEG + cpLocal.iMonthBeg,
                                  BM_SETCHECK, MPFROM2SHORT (TRUE, 0), NULL) ;

               WinSendDlgItemMsg (hwnd, IDD_MONTHEND + cpLocal.iMonthEnd,
                                  BM_SETCHECK, MPFROM2SHORT (TRUE, 0), NULL) ;

               WinSendDlgItemMsg (hwnd, IDD_YEAR, EM_SETTEXTLIMIT,
                                  MPFROM2SHORT (4, 0), NULL) ;

               WinSetDlgItemShort (hwnd, IDD_YEAR, cpLocal.iYear, FALSE) ;
               return 0 ;

          case WM_CONTROL:
               if (SHORT1FROMMP (mp1) >= IDD_MONTHBEG &&
                   SHORT1FROMMP (mp1) <  IDD_MONTHBEG + 12)

                    cpLocal.iMonthBeg = SHORT1FROMMP (mp1) - IDD_MONTHBEG ;

               else if (SHORT1FROMMP (mp1) >= IDD_MONTHEND &&
                        SHORT1FROMMP (mp1) <  IDD_MONTHEND + 12)

                    cpLocal.iMonthEnd = SHORT1FROMMP (mp1) - IDD_MONTHEND ;

               return 0 ;

          case WM_COMMAND:
               switch (COMMANDMSG(&msg)->cmd)
                    {
                    case DID_OK:
                    case IDD_PREVIEW:
                         WinQueryDlgItemShort (hwnd, IDD_YEAR,
                                               &cpLocal.iYear, FALSE) ;

                         if (cpLocal.iYear < 1900 || cpLocal.iYear > 2099)
                              {
                              Message (hwnd, MB_ICONEXCLAMATION,
                                       "Year must be between 1900 and 2099!") ;
                              WinSetFocus (HWND_DESKTOP,
                                           WinWindowFromID (hwnd, IDD_YEAR)) ;
                              return 0 ;
                              }

                         if (cpLocal.iMonthBeg > cpLocal.iMonthEnd)
                              {
                              Message (hwnd, MB_ICONEXCLAMATION,
                                       "Begin month cannot be later "
                                       "than end month!") ;
                              WinSetFocus (HWND_DESKTOP,
                                   WinWindowFromID (hwnd,
                                        IDD_MONTHBEG + cpLocal.iMonthBeg)) ;
                              return 0 ;
                              }

                         *pcpCurrent = cpLocal ;
                                                       // fall through
                    case DID_CANCEL:
                         WinDismissDlg (hwnd, COMMANDMSG(&msg)->cmd) ;
                         return 0 ;
                    }
               break ;
          }
     return WinDefDlgProc (hwnd, msg, mp1, mp2) ;
     }

VOID DisplayPage (HPS hps, SIZEL *psizlPage, INT iYear, INT iMonth)
     {
     static CHAR  *apszMonths[]  = { "January", "February", "March",
                                     "April",   "May",      "June",
                                     "July",    "August",   "September",
                                     "October", "November", "December" } ;
     static INT   aiMonthLen[]   = { 31, 28, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31 } ;
     static INT   aiMonthStart[] = {  0,  3,  3,  6,  1,  4,
                                      6,  2,  5,  0,  3,  5 } ;
     CHAR         szBuffer[16] ;
     BOOL         fLeap ;
     FATTRS       fat ;
     INT          iDayStart, iDay, iExtraDay ;
     LONG         lLength ;
     POINTL       ptl, aptlTextBox[TXTBOX_COUNT] ;
     SIZEF        sizfx ;
     SIZEL        sizlCell ;

     GpiSavePS (hps) ;

               // Determine size of day cell

     sizlCell.cx = (psizlPage->cx - 1) / 7 ;
     sizlCell.cy = (psizlPage->cy - 1) / 7 ;

               // Create the vector font and use it in the PS

     fat.usRecordLength  = sizeof fat ;
     fat.fsSelection     = 0 ;
     fat.lMatch          = 0 ;
     fat.idRegistry      = 0 ;
     fat.usCodePage      = GpiQueryCp (hps) ;
     fat.lMaxBaselineExt = 0 ;
     fat.lAveCharWidth   = 0 ;
     fat.fsType          = 0 ;
     fat.fsFontUse       = FATTR_FONTUSE_OUTLINE |
                           FATTR_FONTUSE_TRANSFORMABLE ;

     strcpy (fat.szFacename, "Helvetica") ;

     GpiCreateLogFont (hps, NULL, LCID_CALFONT, &fat) ;
     GpiSetCharSet (hps, LCID_CALFONT) ;

               // Scale the font for the month and year name

     lLength = sprintf (szBuffer, " %s %d ", apszMonths[iMonth], iYear) ;
     GpiQueryTextBox (hps, lLength, szBuffer, TXTBOX_COUNT, aptlTextBox) ;
     GpiQueryCharBox (hps, &sizfx) ;

     sizfx.cx = sizlCell.cx * sizfx.cx /  aptlTextBox[TXTBOX_CONCAT].x * 7 ;
     sizfx.cy = sizlCell.cy * sizfx.cy / (aptlTextBox[TXTBOX_TOPLEFT].y -
                                          aptlTextBox[TXTBOX_BOTTOMLEFT].y) ;

     sizfx.cx = sizfx.cy = min (sizfx.cx, sizfx.cy) ;
     GpiSetCharBox (hps, &sizfx) ;
     GpiQueryTextBox (hps, lLength, szBuffer, TXTBOX_COUNT, aptlTextBox) ;

               // Display month and year at top of page

     ptl.x = (psizlPage->cx - aptlTextBox[TXTBOX_CONCAT].x) / 2 ;
     ptl.y =  6 * sizlCell.cy - aptlTextBox[TXTBOX_BOTTOMLEFT].y ;
     GpiCharStringAt (hps, &ptl, lLength, szBuffer) ;

               // Set font size for day numbers

     sizfx.cx = sizfx.cy = MAKEFIXED (min (sizlCell.cx, sizlCell.cy) / 4, 0) ;
     GpiSetCharBox (hps, &sizfx) ;

               // Calculate some variables for showing days in month

     fLeap = (iYear % 4 == 0) && ((iYear % 100 != 0) || (iYear % 400 == 0)) ;
     iExtraDay = fLeap && iMonth == 1 ? 1 : 0 ;

     iDayStart  = 1 + iYear - 1900 + (iYear - 1901) / 4 ;
     iDayStart += aiMonthStart[iMonth] + (fLeap && iMonth > 1 ? 1 : 0) ;
     iDayStart %= 7 ;

               // Loop through days

     for (iDay = 0 ; iDay < aiMonthLen[iMonth] + iExtraDay ; iDay++)
          {
          ptl.x =      (iDayStart + iDay) % 7  * sizlCell.cx ;
          ptl.y = (5 - (iDayStart + iDay) / 7) * sizlCell.cy ;
          GpiMove (hps, &ptl) ;

          ptl.x += sizlCell.cx ;
          ptl.y += sizlCell.cy ;
          GpiBox (hps, DRO_OUTLINE, &ptl, 0L, 0L) ;

          lLength = sprintf (szBuffer, " %d", iDay + 1) ;
          GpiQueryTextBox (hps, lLength, szBuffer, TXTBOX_COUNT, aptlTextBox) ;

          GpiQueryCurrentPosition (hps, &ptl) ;
          ptl.y += sizlCell.cy - aptlTextBox[TXTBOX_TOPLEFT].y ;
          GpiCharStringAt (hps, &ptl, lLength, szBuffer) ;
          }
               // Clean up

     GpiSetCharSet (hps, LCID_DEFAULT) ;
     GpiDeleteSetId (hps, LCID_CALFONT) ;
     GpiRestorePS (hps, -1L) ;
     }

VOID Message (HWND hwnd, INT sIcon, CHAR *pszMessage)
     {
     WinMessageBox (HWND_DESKTOP, hwnd, pszMessage, "PrintCal",
                    0, sIcon | MB_OK | MB_MOVEABLE) ;
     }

VOID PrintThread (VOID * pArg)
     {
     HAB            hab ;
     HDC            hdcPrinter ;
     HPS            hpsPrinter ;
     INT            msgReturn ;
     INT            iMonth ;
     SIZEL          sizlPage ;
     THREADPARAMS * ptp ;

     ptp = (THREADPARAMS *) pArg ;

     hab = WinInitialize (0) ;

     if ((hdcPrinter = OpenDefaultPrinterDC (hab)) != DEV_ERROR)
          {
                    // Create the presentation space for the printer

          sizlPage.cx = 0 ;
          sizlPage.cy = 0 ;
          hpsPrinter = GpiCreatePS (hab, hdcPrinter, &sizlPage,
                                    PU_ARBITRARY | GPIF_DEFAULT |
                                    GPIT_MICRO   | GPIA_ASSOC) ;

          GpiQueryPS (hpsPrinter, &sizlPage) ;

                    // Start the document

          if (DevEscape (hdcPrinter, DEVESC_STARTDOC,
                         8L, "Calendar", NULL, NULL) != DEVESC_ERROR)
               {
                        // Loop through months

               for (iMonth  = ptp->cp.iMonthBeg ;
                    iMonth <= ptp->cp.iMonthEnd ; iMonth++)
                    {
                    DisplayPage (hpsPrinter, &sizlPage, ptp->cp.iYear, iMonth);

                    DevEscape (hdcPrinter, DEVESC_NEWFRAME,
                               0L, NULL, NULL, NULL) ;
                    }

                         // End the document

               DevEscape (hdcPrinter, DEVESC_ENDDOC, 0L, NULL, NULL, NULL) ;
               msgReturn = WM_USER_PRINT_OK ;
               }
          else
               msgReturn = WM_USER_PRINT_ERROR ;

                    // Clean up

          GpiDestroyPS (hpsPrinter) ;
          DevCloseDC (hdcPrinter) ;
          }
     else
          msgReturn = WM_USER_PRINT_ERROR ;

               // Post message to client window and end thread

     DosEnterCritSec () ;
     WinPostMsg (ptp->hwndNotify, msgReturn, MPFROMP (ptp), NULL) ;
     WinTerminate (hab) ;
     _endthread () ;
     }
