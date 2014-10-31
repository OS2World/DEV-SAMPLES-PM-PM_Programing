#-----------------------
# OLFSIZE.MAK make file
#-----------------------

olfsize.exe : olfsize.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfsize olfdemo olf, olfsize, NUL, $(PRGLIB), olfdemo

olfsize.obj : olfsize.c olf.h
     $(PRGCC) olfsize.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
