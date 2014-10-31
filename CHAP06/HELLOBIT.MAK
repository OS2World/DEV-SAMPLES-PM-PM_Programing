#------------------------
# HELLOBIT.MAK make file
#------------------------

hellobit.exe : hellobit.obj hellobit.def
     $(PRGLINK) hellobit, hellobit, NUL, $(PRGLIB), hellobit

hellobit.obj : hellobit.c
     $(PRGCC) hellobit.c
