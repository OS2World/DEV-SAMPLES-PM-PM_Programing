REM -------------------------------------------------------------------------
REM  BORMT.CMD -- Set up multithreading environment for Borland C++ for OS/2
REM -------------------------------------------------------------------------
SET PRGCCMT=bcc -c -I\bcos2\include -sm -o
SET PRGLINK=tlink -c -x -v -L\bcos2\lib \bcos2\lib\c02.obj
SET PRGLIBMT=c2mt + os2
SET PRGRC=rc -r -i \bcos2\include
