#-----------------------
# KEYLOOK.MAK make file
#-----------------------

keylook.exe : keylook.obj easyfont.obj keylook.def
     $(PRGLINK) keylook easyfont, keylook, NUL, $(PRGLIB), keylook

keylook.obj : keylook.c easyfont.h
     $(PRGCC) keylook.c

easyfont.obj : easyfont.c easyfont.h
     $(PRGCC) easyfont.c
