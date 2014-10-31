#------------------------
# CHECKER3.MAK make file
#------------------------

checker3.exe : checker3.obj checker3.def
     $(PRGLINK) checker3, checker3, NUL, $(PRGLIB), checker3

checker3.obj : checker3.c
     $(PRGCC) checker3.c
