#------------------------
# OLFSHEAR.MAK make file
#------------------------

olfshear.exe : olfshear.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfshear olfdemo olf, olfshear, NUL, $(PRGLIB), olfdemo

olfshear.obj : olfshear.c olf.h
     $(PRGCC) olfshear.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
