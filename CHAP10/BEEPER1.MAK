#-----------------------
# BEEPER1.MAK make file
#-----------------------  

beeper1.exe : beeper1.obj beeper1.def
     $(PRGLINK) beeper1, beeper1, NUL, $(PRGLIB), beeper1

beeper1.obj : beeper1.c
     $(PRGCC) beeper1.c
