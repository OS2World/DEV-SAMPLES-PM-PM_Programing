#----------------------
# CIRCLE.MAK make file
#----------------------

circle.exe : circle.obj circle.def
     $(PRGLINK) circle, circle, NUL, $(PRGLIB), circle

circle.obj : circle.c
     $(PRGCC) circle.c
