/*----------------------------------------
   HEXCALC.C -- Hexadecimal Calculator
                (c) Charles Petzold, 1993
  ----------------------------------------*/

#define INCL_WIN
#include <os2.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "hexcalc.h"

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
                 WinLoadPointer (HWND_DESKTOP, 0, ID_ICON), NULL) ;

     WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwndFrame, FID_CLIENT)) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
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
     static BOOL  fNewNumber = TRUE ;
     static INT   iOperation = '=' ;
     static ULONG ulNumber, ulFirstNum ;
     HWND         hwndButton ;
     INT          idButton ;

     switch (msg)
          {
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

               if (idButton == '\b')                        // backspace
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
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
