#-----------------------
# OLFWIDE.MAK make file
#-----------------------

olfwide.exe : olfwide.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfwide olfdemo olf, olfwide, NUL, $(PRGLIB), olfdemo

olfwide.obj : olfwide.c olf.h
     $(PRGCC) olfwide.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
