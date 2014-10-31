#-----------------------
# OLFLIST.MAK make file
#-----------------------

olflist.exe : olflist.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olflist olfdemo olf, olflist, NUL, $(PRGLIB), olfdemo

olflist.obj : olflist.c olf.h
     $(PRGCC) olflist.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
