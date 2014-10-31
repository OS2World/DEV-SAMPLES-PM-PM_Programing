#-----------------------
# HEXCALC.MAK make file
#-----------------------

hexcalc.exe : hexcalc.obj hexcalc.def hexcalc.res
     $(PRGLINK) hexcalc, hexcalc, NUL, $(PRGLIB), hexcalc
     rc hexcalc.res

hexcalc.obj: hexcalc.c hexcalc.h
     $(PRGCC) hexcalc.c

hexcalc.res : hexcalc.rc hexcalc.h
     $(PRGRC) hexcalc.rc
