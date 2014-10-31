#------------------------
# WELCOME3.MAK make file
#------------------------

welcome3.exe : welcome3.obj welcome3.def
     $(PRGLINK) welcome3, welcome3, NUL, $(PRGLIB), welcome3

welcome3.obj : welcome3.c
     $(PRGCC) welcome3.c
