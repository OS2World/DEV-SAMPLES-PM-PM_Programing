#-----------------------
# MINMAX2.MAK make file
#-----------------------

minmax2.exe : minmax2.obj minmax2.def
     $(PRGLINK) minmax2, minmax2, NUL, $(PRGLIB), minmax2

minmax2.obj : minmax2.c
     $(PRGCC) minmax2.c
