#-----------------------
# OLFLINE.MAK make file
#-----------------------

olfline.exe : olfline.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfline olfdemo olf, olfline, NUL, $(PRGLIB), olfdemo

olfline.obj : olfline.c olf.h
     $(PRGCC) olfline.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
