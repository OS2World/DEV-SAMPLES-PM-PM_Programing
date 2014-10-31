#----------------------
# SPIRAL.MAK make file
#----------------------

spiral.exe : spiral.obj spiral.def
     $(PRGLINK) spiral, spiral, NUL, $(PRGLIB), spiral

spiral.obj : spiral.c
     $(PRGCC) spiral.c
