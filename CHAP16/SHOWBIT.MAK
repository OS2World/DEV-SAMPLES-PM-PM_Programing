#-----------------------
# SHOWBIT.MAK make file
#-----------------------

showbit.exe : showbit.obj showbit.def
     $(PRGLINK) showbit, showbit, NUL, $(PRGLIB), showbit

showbit.obj : showbit.c
     $(PRGCC) showbit.c
