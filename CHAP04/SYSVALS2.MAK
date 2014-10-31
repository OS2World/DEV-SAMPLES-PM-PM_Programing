#------------------------
# SYSVALS2.MAK make file
#------------------------

sysvals2.exe : sysvals2.obj sysvals2.def
     $(PRGLINK) sysvals2, sysvals2, NUL, $(PRGLIB), sysvals2

sysvals2.obj : sysvals2.c sysvals.h
     $(PRGCC) sysvals2.c
