#----------------------
# SKETCH.MAK make file
#----------------------

sketch.exe : sketch.obj sketch.def
     $(PRGLINK) sketch, sketch, NUL, $(PRGLIB), sketch

sketch.obj : sketch.c
     $(PRGCC) sketch.c
