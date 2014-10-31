#------------------------
# WELCOME4.MAK make file
#------------------------

welcome4.exe : welcome4.obj welcome4.def
     $(PRGLINK) welcome4, welcome4, NUL, $(PRGLIB), welcome4

welcome4.obj : welcome4.c
     $(PRGCC) welcome4.c
