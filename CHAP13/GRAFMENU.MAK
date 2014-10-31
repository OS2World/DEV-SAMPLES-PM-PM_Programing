#------------------------
# GRAFMENU.MAK make file
#------------------------

grafmenu.exe : grafmenu.obj grafmenu.def grafmenu.res
     $(PRGLINK) grafmenu, grafmenu, NUL, $(PRGLIB), grafmenu
     rc grafmenu.res

grafmenu.obj : grafmenu.c grafmenu.h
     $(PRGCC) grafmenu.c

grafmenu.res : grafmenu.rc grafmenu.h bighelp.bmp
     $(PRGRC) grafmenu
