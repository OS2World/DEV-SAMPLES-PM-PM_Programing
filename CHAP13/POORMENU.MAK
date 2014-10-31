#------------------------
# POORMENU.MAK make file
#------------------------

poormenu.exe : poormenu.obj poormenu.def
     $(PRGLINK) poormenu, poormenu, NUL, $(PRGLIB), poormenu

poormenu.obj : poormenu.c
     $(PRGCC) poormenu.c
