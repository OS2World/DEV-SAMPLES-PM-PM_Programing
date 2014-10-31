REM ----------------------------------------------------------------------
REM  IBMDLL.CMD -- Set up DLL environment for IBM C Developer's WorkSet/2
REM ----------------------------------------------------------------------
SET DLLCC=icc -C -Kbcpr -Ss -Ge- -Fo
SET DLLLINK=link386
SET DLLLIB=
SET DLLRC=rc -r
