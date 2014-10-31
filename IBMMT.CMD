REM ------------------------------------------------------------------------
REM  IBMMT.CMD -- Multthreading environment for IBM C Developer's WorkSet/2
REM ------------------------------------------------------------------------
SET PRGCCMT=icc -C -Kbcpr -Ss -Gm -Fo
SET PRGLINK=link386 /BASE:0x10000
SET PRGLIBMT=
SET PRGRC=rc -r
