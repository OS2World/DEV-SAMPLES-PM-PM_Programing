#-----------------------
# BITCAT2.MAK make file
#-----------------------

bitcat2.exe : bitcat2.obj bitcat2.def
     $(PRGLINK) bitcat2, bitcat2, NUL, $(PRGLIB), bitcat2

bitcat2.obj : bitcat2.c bitcat.h
     $(PRGCC) bitcat2.c
