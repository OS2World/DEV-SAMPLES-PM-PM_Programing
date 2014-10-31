#-----------------------
# ALTWIND.MAK make file
#-----------------------

altwind.exe : altwind.obj altwind.def
     $(PRGLINK) altwind, altwind, NUL, $(PRGLIB), altwind

altwind.obj : altwind.c
     $(PRGCC) altwind.c
