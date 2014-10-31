#------------------------
# CHECKER1.MAK make file
#------------------------

checker1.exe : checker1.obj checker1.def
     $(PRGLINK) checker1, checker1, NUL, $(PRGLIB), checker1

checker1.obj : checker1.c
     $(PRGCC) checker1.c
