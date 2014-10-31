REM ---------------------------------------------------------------
REM  IBM.CMD -- Set up environment for IBM C Developer's WorkSet/2
REM ---------------------------------------------------------------
SET PRGCC=icc -C -Kbcpr -Ss
SET PRGLINK=link386 /BASE:0x10000
SET PRGLIB=
SET PRGRC=rc -r
