#-----------------------
# OLFSTR2.MAK make file
#-----------------------

olfstr2.exe : olfstr2.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfstr2 olfdemo olf, olfstr2, NUL, $(PRGLIB), olfdemo

olfstr2.obj : olfstr2.c olf.h
     $(PRGCC) olfstr2.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
