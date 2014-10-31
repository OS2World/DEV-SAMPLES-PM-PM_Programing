#-----------------------
# OLFDROP.MAK make file
#-----------------------

olfdrop.exe : olfdrop.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfdrop olfdemo olf, olfdrop, NUL, $(PRGLIB), olfdemo

olfdrop.obj : olfdrop.c olf.h
     $(PRGCC) olfdrop.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
