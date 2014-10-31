#-----------------------
# MINMAX1.MAK make file
#-----------------------

minmax1.exe : minmax1.obj minmax1.def
     $(PRGLINK) minmax1, minmax1, NUL, $(PRGLIB), minmax1

minmax1.obj : minmax1.c
     $(PRGCC) minmax1.c
