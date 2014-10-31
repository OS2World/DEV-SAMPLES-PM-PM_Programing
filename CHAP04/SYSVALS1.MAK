#------------------------
# SYSVALS1.MAK make file
#------------------------

sysvals1.exe : sysvals1.obj sysvals1.def
     $(PRGLINK) sysvals1, sysvals1, NUL, $(PRGLIB), sysvals1

sysvals1.obj : sysvals1.c sysvals.h
     $(PRGCC) sysvals1.c
