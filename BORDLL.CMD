REM ---------------------------------------------------------------
REM  BORDLL.CMD -- Set up DLL environment for Borland C++ for OS/2
REM ---------------------------------------------------------------
SET DLLCC=bcc -c -I\bcos2\include -sd -o
SET DLLLINK=tlink -c -x -v -L\bcos2\lib \bcos2\lib\c02d.obj
SET DLLLIB=c2 + os2
SET DLLRC=rc -r -i \bcos2\include
