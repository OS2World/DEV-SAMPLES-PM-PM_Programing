#-----------------------
# OLFJUST.MAK make file
#-----------------------

olfjust.exe : olfjust.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfjust olfdemo olf, olfjust, NUL, $(PRGLIB), olfdemo

olfjust.obj : olfjust.c olf.h
     $(PRGCC) olfjust.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
