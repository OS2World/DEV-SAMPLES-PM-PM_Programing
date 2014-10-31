#------------------
# WE.MAK make file
#------------------

we.exe : we.obj we.def
     $(PRGLINK) we, we, NUL, $(PRGLIB), we

we.obj : we.c
     $(PRGCC) we.c
