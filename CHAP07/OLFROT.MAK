#----------------------
# OLFROT.MAK make file
#----------------------

olfrot.exe : olfrot.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfrot olfdemo olf, olfrot, NUL, $(PRGLIB), olfdemo

olfrot.obj : olfrot.c olf.h
     $(PRGCC) olfrot.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
