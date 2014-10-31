#-----------------------
# BEEPER2.MAK make file
#-----------------------

beeper2.exe : beeper2.obj beeper2.def
     $(PRGLINK) beeper2, beeper2, NUL, $(PRGLIB), beeper2

beeper2.obj : beeper2.c
     $(PRGCC) beeper2.c
