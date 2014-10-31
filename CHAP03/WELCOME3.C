/*-----------------------------------------------------------
   WELCOME3.C -- Creates a Top-Level Window and Two Children
                 (c) Charles Petzold, 1993
  -----------------------------------------------------------*/

#define INCL_WIN
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
MRESULT EXPENTRY ChildWndProc  (HWND, ULONG, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass [] = "Welcome3",
                  szChildClass  [] = "Welcome3.Child" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame,  hwndClient, hwndChildClient1, hwndChildClient2 ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (
                    hab,                // Anchor block handle
                    szClientClass,      // Name of class being registered
                    ClientWndProc,      // Window procedure for class
                    CS_SIZEREDRAW,      // Class style
                    0) ;                // Extra bytes to reserve

     WinRegisterClass (
                    hab,                // Anchor block handle
                    szChildClass,       // Name of class being registered
                    ChildWndProc,       // Window procedure for class
                    CS_SIZEREDRAW,      // Class style
                    sizeof (PVOID)) ;   // Extra bytes to reserve

          /*-------------------------
             Create top-level window
            -------------------------*/

     hwndFrame = WinCreateStdWindow (
                    HWND_DESKTOP,       // Parent window handle
                    WS_VISIBLE,         // Style of frame window
                    &flFrameFlags,      // Pointer to control data
                    szClientClass,      // Client window class name
                    NULL,               // Title bar text
                    0L,                 // Style of client window
                    0,                  // Module handle for resources
                    0,                  // ID of resources
                    &hwndClient) ;      // Pointer to client window handle

          /*--------------------------
             Create two child windows
            --------------------------*/

     flFrameFlags &= ~FCF_TASKLIST ;

     WinCreateStdWindow (
                    hwndClient,         // Parent window handle
                    WS_VISIBLE,         // Style of frame window
                    &flFrameFlags,      // Pointer to control data
                    szChildClass,       // Client window class name
                    "Child No. 1",      // Title bar text
                    0L,                 // Style of client window
                    0,                  // Module handle for resources
                    0,                  // ID of resources
                    &hwndChildClient1) ;// Pointer to client window handle

     WinCreateStdWindow (
                    hwndClient,         // Parent window handle
                    WS_VISIBLE,         // Style of frame window
                    &flFrameFlags,      // Pointer to control data
                    szChildClass,       // Client window class name
                    "Child No. 2",      // Title bar text
                    0L,                 // Style of client window
                    0,                  // Module handle for resources
                    0,                  // ID of resources
                    &hwndChildClient2) ;// Pointer to client window handle

          /*-----------------------------------------------------
             Set reserved area of window to text string pointers
            -----------------------------------------------------*/

     WinSetWindowPtr (hwndChildClient1, QWL_USER, "I'm a child ...") ;
     WinSetWindowPtr (hwndChildClient2, QWL_USER, "... Me too!") ;      

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static CHAR szText [] = "I'm the parent of two children" ;
     HPS         hps ;
     RECTL       rcl ;

     switch (msg)
          {
          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               WinQueryWindowRect (hwnd, &rcl) ;

               WinDrawText (hps, -1, szText, &rcl, CLR_NEUTRAL, CLR_BACKGROUND,
                            DT_CENTER | DT_VCENTER | DT_ERASERECT) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

MRESULT EXPENTRY ChildWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     HPS   hps ;
     RECTL rcl ;

     switch (msg)
          {
          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

               WinQueryWindowRect (hwnd, &rcl) ;

               WinDrawText (hps, -1, WinQueryWindowPtr (hwnd, QWL_USER), &rcl,
                            CLR_NEUTRAL, CLR_BACKGROUND,
                            DT_CENTER | DT_VCENTER | DT_ERASERECT) ;

               WinEndPaint (hps) ;
               return 0 ;

          case WM_CLOSE:
               WinDestroyWindow (WinQueryWindow (hwnd, QW_PARENT)) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
