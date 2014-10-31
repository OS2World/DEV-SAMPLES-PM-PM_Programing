#------------------------
# WELCOME2.MAK make file
#------------------------

welcome2.exe : welcome2.obj welcome2.def
     $(PRGLINK) welcome2, welcome2, NUL, $(PRGLIB), welcome2

welcome2.obj : welcome2.c
     $(PRGCC) welcome2.c
