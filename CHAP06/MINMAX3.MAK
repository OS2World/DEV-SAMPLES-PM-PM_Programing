#-----------------------
# MINMAX3.MAK make file
#-----------------------

minmax3.exe : minmax3.obj minmax3.def
     $(PRGLINK) minmax3, minmax3, NUL, $(PRGLIB), minmax3

minmax3.obj : minmax3.c
     $(PRGCC) minmax3.c
