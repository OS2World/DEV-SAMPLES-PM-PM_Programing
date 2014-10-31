#-----------------------
# OLFSTR1.MAK make file
#-----------------------

olfstr1.exe : olfstr1.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfstr1 olfdemo olf, olfstr1, NUL, $(PRGLIB), olfdemo

olfstr1.obj : olfstr1.c olf.h
     $(PRGCC) olfstr1.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
