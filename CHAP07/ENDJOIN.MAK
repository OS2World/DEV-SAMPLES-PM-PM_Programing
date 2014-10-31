#-----------------------
# ENDJOIN.MAK make file
#-----------------------

endjoin.exe : endjoin.obj endjoin.def
     $(PRGLINK) endjoin, endjoin, NUL, $(PRGLIB), endjoin

endjoin.obj : endjoin.c
     $(PRGCC) endjoin.c
