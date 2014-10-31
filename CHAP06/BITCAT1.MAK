#-----------------------
# BITCAT1.MAK make file
#-----------------------

bitcat1.exe : bitcat1.obj bitcat1.def
     $(PRGLINK) bitcat1, bitcat1, NUL, $(PRGLIB), bitcat1

bitcat1.obj : bitcat1.c bitcat.h
     $(PRGCC) bitcat1.c
