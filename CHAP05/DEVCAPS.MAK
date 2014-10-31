#-----------------------
# DEVCAPS.MAK make file
#-----------------------

devcaps.exe : devcaps.obj devcaps.def
     $(PRGLINK) devcaps, devcaps, NUL, $(PRGLIB), devcaps

devcaps.obj : devcaps.c devcaps.h
     $(PRGCC) devcaps.c
