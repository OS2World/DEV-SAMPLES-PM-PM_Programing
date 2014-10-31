#------------------------
# SYSVALS3.MAK make file
#------------------------

sysvals3.exe : sysvals3.obj sysvals3.def
     $(PRGLINK) sysvals3, sysvals3, NUL, $(PRGLIB), sysvals3

sysvals3.obj : sysvals3.c sysvals.h
     $(PRGCC) sysvals3.c
