#-----------------
# W.MAK make file
#-----------------

w.exe : w.obj w.def
     $(PRGLINK) w, w, NUL, $(PRGLIB), w

w.obj : w.c
     $(PRGCC) w.c
