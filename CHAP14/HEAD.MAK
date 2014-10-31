#--------------------
# HEAD.MAK make file
#--------------------

head.exe : head.obj easyfont.obj head.def head.res
     $(PRGLINK) head easyfont, head, NUL, $(PRGLIB), head
     rc head.res

head.obj : head.c head.h easyfont.h
     $(PRGCC) head.c

easyfont.obj : easyfont.c
     $(PRGCC) easyfont.c

head.res : head.rc head.h
     $(PRGRC) head
