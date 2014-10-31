REM --------------------------------------------------------
REM  BOR.CMD -- Set up environment for Borland C++ for OS/2
REM --------------------------------------------------------
SET PRGCC=bcc -c -I\bcos2\include
SET PRGLINK=tlink -c -L\bcos2\lib \bcos2\lib\c02.obj
SET PRGLIB=c2 + os2
SET PRGRC=rc -r -i \bcos2\include
