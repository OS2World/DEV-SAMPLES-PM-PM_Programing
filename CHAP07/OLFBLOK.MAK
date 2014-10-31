#-----------------------
# OLFBLOK.MAK make file
#-----------------------

olfblok.exe : olfblok.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfblok olfdemo olf, olfblok, NUL, $(PRGLIB), olfdemo

olfblok.obj : olfblok.c olf.h
     $(PRGCC) olfblok.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
