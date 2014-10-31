#--------------------
# LIFE.MAK make file
#--------------------

life.exe : life.obj life.def life.res
     $(PRGLINK) life, life, NUL, $(PRGLIB), life
     rc life.res

life.obj : life.c life.h
     $(PRGCC) life.c

life.res : life.rc life.h life.ico
     $(PRGRC) life
