#----------------------
# BEZIER.MAK make file
#----------------------

bezier.exe : bezier.obj bezier.def
     $(PRGLINK) bezier, bezier, NUL, $(PRGLIB), bezier

bezier.obj : bezier.c
     $(PRGCC) bezier.c
