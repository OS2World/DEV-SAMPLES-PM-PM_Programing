#------------------------
# LINETYPE.MAK make file
#------------------------

linetype.exe : linetype.obj linetype.def
     $(PRGLINK) linetype, linetype, NUL, $(PRGLIB), linetype

linetype.obj : linetype.c
     $(PRGCC) linetype.c
