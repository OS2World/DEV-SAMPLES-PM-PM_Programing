#-----------------------
# OLFSHAD.MAK make file
#-----------------------

olfshad.exe : olfshad.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfshad olfdemo olf, olfshad, NUL, $(PRGLIB), olfdemo

olfshad.obj : olfshad.c olf.h
     $(PRGCC) olfshad.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
