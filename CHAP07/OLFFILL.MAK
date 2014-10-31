#-----------------------
# OLFFILL.MAK make file
#-----------------------

olffill.exe : olffill.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olffill olfdemo olf, olffill, NUL, $(PRGLIB), olfdemo

olffill.obj : olffill.c olf.h
     $(PRGCC) olffill.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
