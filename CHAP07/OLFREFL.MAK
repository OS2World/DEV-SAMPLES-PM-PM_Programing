#-----------------------
# OLFREFL.MAK make file
#-----------------------

olfrefl.exe : olfrefl.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfrefl olfdemo olf, olfrefl, NUL, $(PRGLIB), olfdemo

olfrefl.obj : olfrefl.c olf.h
     $(PRGCC) olfrefl.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
