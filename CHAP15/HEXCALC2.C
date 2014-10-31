/*-------------------------------------------------------------------
   HEXCALC2.C -- Hexadecimal Calculator with Clipboard Cut and Paste
                 (c) Charles Petzold, 1993
  -------------------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexcalc.h"

#define IDM_COPY    256
#define IDM_PASTE   257

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     HAB  hab ;
     HMQ  hmq ;
     HWND hwndFrame ;
     QMSG qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, CLIENTCLASS, ClientWndProc, 0L, 0) ;

     hwndFrame = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP,
                             NULL, NULLHANDLE, ID_HEXCALC, NULL) ;

     WinSendMsg (hwndFrame, WM_SETICON, (MPARAM)
                 WinLoadPointer (HWND_DESKTOP, NULLHANDLE, ID_ICON), NULL) ;

     WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwndFrame, FID_CLIENT)) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

HACCEL AddItemsToSysMenu (HAB hab, HWND hwndFrame)
     {
     static CHAR     *szMenuText [3] = { NULL, "~Copy\tCtrl+Ins",
                                               "~Paste\tShift+Ins" } ;
     static MENUITEM mi [3] = {
                              MIT_END, MIS_SEPARATOR, 0, 0,         0, 0,
                              MIT_END, MIS_TEXT,      0, IDM_COPY,  0, 0,
                              MIT_END, MIS_TEXT,      0, IDM_PASTE, 0, 0
                              } ;
     ACCELTABLE      *pacct ;
     HACCEL          haccel ;
     HWND            hwndSysMenu, hwndSysSubMenu ;
     INT             idSysMenu, iItem ;
     MENUITEM        miSysMenu ;

                              // Add items to system menu

     hwndSysMenu = WinWindowFromID (hwndFrame, FID_SYSMENU) ;
     idSysMenu = SHORT1FROMMR (WinSendMsg (hwndSysMenu,
                                           MM_ITEMIDFROMPOSITION,
                                           NULL, NULL)) ;

     WinSendMsg (hwndSysMenu, MM_QUERYITEM,
                 MPFROM2SHORT (idSysMenu, FALSE),
                 MPFROMP (&miSysMenu)) ;

     hwndSysSubMenu = miSysMenu.hwndSubMenu ;

     for (iItem = 0 ; iItem < 3 ; iItem++)
          WinSendMsg (hwndSysSubMenu, MM_INSERTITEM,
                      MPFROMP (mi + iItem),
                      MPFROMP (szMenuText [iItem])) ;

                              // Create and set accelerator table

     pacct = malloc (sizeof (ACCELTABLE) + sizeof (ACCEL)) ;

     pacct->cAccel        = 2 ;    // Number of accelerators
     pacct->codepage      = 0 ;    // Not used

     pacct->aaccel[0].fs  = AF_VIRTUALKEY | AF_CONTROL ;
     pacct->aaccel[0].key = VK_INSERT ;
     pacct->aaccel[0].cmd = IDM_COPY ;

     pacct->aaccel[1].fs  = AF_VIRTUALKEY | AF_SHIFT ;
     pacct->aaccel[1].key = VK_INSERT ;
     pacct->aaccel[1].cmd = IDM_PASTE ;

     haccel = WinCreateAccelTable (hab, pacct) ;
     WinSetAccelTable (hab, haccel, hwndFrame) ;

     free (pacct) ;

     return haccel ;
     }

VOID EnableSysMenuItem (HWND hwnd, INT idItem, BOOL fEnable)
     {
     HWND hwndSysMenu ;

     hwndSysMenu = WinWindowFromID (WinQueryWindow (hwnd, QW_PARENT),
                                    FID_SYSMENU) ;

     WinSendMsg (hwndSysMenu, MM_SETITEMATTR,
                 MPFROM2SHORT ((USHORT) idItem, TRUE),
                 MPFROM2SHORT (MIA_DISABLED, fEnable ? 0 : MIA_DISABLED)) ;
     }

void ShowNumber (HWND hwnd, ULONG ulNumber)
     {
     CHAR szBuffer [20] ;

     sprintf (szBuffer, "%lX", ulNumber) ;

     WinSetWindowText (WinWindowFromID (hwnd, ESCAPE), szBuffer) ;
     }

ULONG CalcIt (ULONG ulFirstNum, INT iOperation, ULONG ulNum)
     {
     switch (iOperation)
          {
          case '=' : return ulNum ;
          case '+' : return ulFirstNum +  ulNum ;
          case '-' : return ulFirstNum -  ulNum ;
          case '*' : return ulFirstNum *  ulNum ;
          case '&' : return ulFirstNum &  ulNum ;
          case '|' : return ulFirstNum |  ulNum ;
          case '^' : return ulFirstNum ^  ulNum ;
          case '<' : return ulFirstNum << ulNum ;
          case '>' : return ulFirstNum >> ulNum ;
          case '/' : return ulNum ? ulFirstNum / ulNum : ULONG_MAX ;
          case '%' : return ulNum ? ulFirstNum % ulNum : ULONG_MAX ;
          }
     return 0L ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static BOOL   fNewNumber = TRUE ;
     static HAB    hab ;
     static HACCEL haccel ;
     static INT    iOperation = '=' ;
     static ULONG  ulNumber, ulFirstNum ;
     HWND          hwndButton ;
     INT           i, iLen, idButton ;
     PCHAR         pchClipText ;
     QMSG          qmsg ;

     switch (msg)
          {
          case WM_CREATE:
               hab = WinQueryAnchorBlock (hwnd) ;
               haccel = AddItemsToSysMenu (hab,
                              WinQueryWindow (hwnd, QW_PARENT)) ;
               return 0 ;

          case WM_CHAR:
               if (CHARMSG(&msg)->fs & KC_KEYUP)
                    return 0 ;

               if (CHARMSG(&msg)->fs & KC_VIRTUALKEY)
                    switch (CHARMSG(&msg)->vkey)
                         {
                         case VK_LEFT:
                              if (!(CHARMSG(&msg)->fs & KC_CHAR))
                                   {
                                   CHARMSG(&msg)->chr = '\b' ;
                                   CHARMSG(&msg)->fs |= KC_CHAR ;
                                   }
                              break ;

                         case VK_ESC:
                              CHARMSG(&msg)->chr = ESCAPE ;
                              CHARMSG(&msg)->fs |= KC_CHAR ;
                              break ;

                         case VK_NEWLINE:
                         case VK_ENTER:
                              CHARMSG(&msg)->chr = '=' ;
                              CHARMSG(&msg)->fs |= KC_CHAR ;
                              break ;
                         }

               if (CHARMSG(&msg)->fs & KC_CHAR)
                    {
                    CHARMSG(&msg)->chr = toupper (CHARMSG(&msg)->chr) ;

                    hwndButton = WinWindowFromID (hwnd, CHARMSG(&msg)->chr) ;

                    if (hwndButton != NULLHANDLE)
                         WinSendMsg (hwndButton, BM_CLICK, NULL, NULL) ;
                    else
                         WinAlarm (HWND_DESKTOP, WA_ERROR) ;
                    }
               return 0 ;

          case WM_COMMAND:
               idButton = COMMANDMSG(&msg)->cmd ;

               if (idButton == IDM_COPY)                    // "Copy"
                    {
                    hwndButton = WinWindowFromID (hwnd, ESCAPE) ;
                    iLen = WinQueryWindowTextLength (hwndButton) + 1 ;

                    DosAllocSharedMem ((PVOID) &pchClipText, NULL, (ULONG) iLen,
                                       PAG_COMMIT | PAG_READ | PAG_WRITE |
                                       OBJ_TILE | OBJ_GIVEABLE) ;

                    WinQueryWindowText (hwndButton, iLen, pchClipText) ;

                    WinOpenClipbrd (hab) ;
                    WinEmptyClipbrd (hab) ;
                    WinSetClipbrdData (hab, (ULONG) pchClipText, CF_TEXT,
                                       CFI_POINTER) ;
                    WinCloseClipbrd (hab) ;
                    }

               else if (idButton == IDM_PASTE)              // "Paste"
                    {
                    EnableSysMenuItem (hwnd, IDM_COPY,  FALSE) ;
                    EnableSysMenuItem (hwnd, IDM_PASTE, FALSE) ;

                    WinOpenClipbrd (hab) ;

                    pchClipText = (PVOID) WinQueryClipbrdData (hab, CF_TEXT) ;

                    if (pchClipText != 0)
                         {
                         for (i = 0 ; pchClipText[i] ; i++)
                              {
                              if (pchClipText[i] == '\r')
                                   WinSendMsg (hwnd, WM_CHAR,
                                               MPFROM2SHORT (KC_CHAR, 1),
                                               MPFROM2SHORT ('=', 0)) ;

                              else if (pchClipText[i] != '\n' &&
                                       pchClipText[i] != ' ')
                                   WinSendMsg (hwnd, WM_CHAR,
                                               MPFROM2SHORT (KC_CHAR, 1),
                                               MPFROM2SHORT (pchClipText[i],
                                                             0)) ;

                              while (WinPeekMsg (hab, &qmsg, NULLHANDLE,
                                                 0, 0, PM_NOREMOVE))
                                   {
                                   if (qmsg.msg == WM_QUIT)
                                        {
                                        WinCloseClipbrd (hab) ;
                                        return 0 ;
                                        }
                                   else
                                        {
                                        WinGetMsg (hab, &qmsg, NULLHANDLE,
                                                               0, 0) ;
                                        WinDispatchMsg (hab, &qmsg) ;
                                        }
                                   }
                              }
                         }
                    WinCloseClipbrd (hab) ;

                    EnableSysMenuItem (hwnd, IDM_COPY,  TRUE) ;
                    EnableSysMenuItem (hwnd, IDM_PASTE, TRUE) ;
                    }

               else if (idButton == '\b')                   // backspace
                    ShowNumber (hwnd, ulNumber /= 16) ;

               else if (idButton == ESCAPE)                 // escape
                    ShowNumber (hwnd, ulNumber = 0L) ;

               else if (isxdigit (idButton))                // hex digit
                    {
                    if (fNewNumber)
                         {
                         ulFirstNum = ulNumber ;
                         ulNumber = 0L ;
                         }
                    fNewNumber = FALSE ;

                    if (ulNumber <= ULONG_MAX >> 4)
                         ShowNumber (hwnd,
                              ulNumber = 16 * ulNumber + idButton -
                                   (isdigit (idButton) ? '0' : 'A' - 10)) ;
                    else
                         WinAlarm (HWND_DESKTOP, WA_ERROR) ;
                    }
               else                                         // operation
                    {
                    if (!fNewNumber)
                         ShowNumber (hwnd, ulNumber =
                              CalcIt (ulFirstNum, iOperation, ulNumber)) ;
                    fNewNumber = TRUE ;
                    iOperation = idButton ;
                    }
               return 0 ;

          case WM_BUTTON1DOWN:
               WinAlarm (HWND_DESKTOP, WA_ERROR) ;
               break ;

          case WM_ERASEBACKGROUND:
               return MRFROMSHORT (1) ;

          case WM_DESTROY:
               WinDestroyAccelTable (haccel) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
