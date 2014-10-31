#------------------------
# CHECKER2.MAK make file
#------------------------

checker2.exe : checker2.obj checker2.def
     $(PRGLINK) checker2, checker2, NUL, $(PRGLIB), checker2

checker2.obj : checker2.c
     $(PRGCC) checker2.c
