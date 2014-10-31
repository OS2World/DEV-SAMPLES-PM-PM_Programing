#-----------------------
# POPMENU.MAK make file
#-----------------------

popmenu.exe : popmenu.obj popmenu.def popmenu.res
     $(PRGLINK) popmenu, popmenu, NUL, $(PRGLIB), popmenu
     rc popmenu.res

popmenu.obj : popmenu.c popmenu.h
     $(PRGCC) popmenu.c

popmenu.res : popmenu.rc popmenu.h
     $(PRGRC) popmenu
