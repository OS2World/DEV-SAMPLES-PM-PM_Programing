#------------------------
# CONVMENU.MAK make file
#------------------------

convmenu.exe : convmenu.obj convmenu.def convmenu.res
     $(PRGLINK) convmenu, convmenu, NUL, $(PRGLIB), convmenu
     rc convmenu.res

convmenu.obj : convmenu.c convmenu.h
     $(PRGCC) convmenu.c

convmenu.res : convmenu.rc convmenu.h
     $(PRGRC) convmenu
