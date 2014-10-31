#------------------------
# OLFREFL2.MAK make file
#------------------------

olfrefl2.exe : olfrefl2.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfrefl2 olfdemo olf, olfrefl2, NUL, $(PRGLIB), olfdemo

olfrefl2.obj : olfrefl2.c olf.h
     $(PRGCC) olfrefl2.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
