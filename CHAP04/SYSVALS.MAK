#------------------------
# SYSVALS.MAK make file
#------------------------

sysvals.exe : sysvals.obj sysvals.def
     $(PRGLINK) sysvals, sysvals, NUL, $(PRGLIB), sysvals

sysvals.obj : sysvals.c sysvals.h
     $(PRGCC) sysvals.c
