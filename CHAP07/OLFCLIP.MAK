#-----------------------
# OLFCLIP.MAK make file
#-----------------------

olfclip.exe : olfclip.obj olfdemo.obj olf.obj olfdemo.def
     $(PRGLINK) olfclip olfdemo olf, olfclip, NUL, $(PRGLIB), olfdemo

olfclip.obj : olfclip.c olf.h
     $(PRGCC) olfclip.c

olfdemo.obj : olfdemo.c
     $(PRGCC) olfdemo.c

olf.obj : olf.c olf.h
     $(PRGCC) olf.c
