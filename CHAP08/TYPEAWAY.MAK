#------------------------
# TYPEAWAY.MAK make file
#------------------------

typeaway.exe : typeaway.obj easyfont.obj typeaway.def
     $(PRGLINK) typeaway easyfont, typeaway, NUL, $(PRGLIB), typeaway

typeaway.obj : typeaway.c easyfont.h
     $(PRGCC) typeaway.c

easyfont.obj : easyfont.c easyfont.h
     $(PRGCC) easyfont.c
