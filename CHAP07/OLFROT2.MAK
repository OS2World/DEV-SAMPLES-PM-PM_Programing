#-----------------------
# OLFROT2.MAK make file
#-----------------------

olfrot2.exe : olfrot2.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfrot2 olfdemo olf, olfrot2, NUL, $(PRGLIB), olfdemo

olfrot2.obj : olfrot2.c olf.h
     $(PRGCC) olfrot2.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
