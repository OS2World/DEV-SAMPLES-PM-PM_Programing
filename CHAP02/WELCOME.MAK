#-----------------------
# WELCOME.MAK make file
#-----------------------

welcome.exe : welcome.obj welcome.def
     $(PRGLINK) welcome, welcome, NUL, $(PRGLIB), welcome

welcome.obj : welcome.c
     $(PRGCC) welcome.c
