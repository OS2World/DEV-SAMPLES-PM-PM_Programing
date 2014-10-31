#----------------------
# FLOWER.MAK make file
#----------------------

flower.exe : flower.obj flower.def
     $(PRGLINK) flower, flower, NUL, $(PRGLIB), flower

flower.obj : flower.c
     $(PRGCC) flower.c
