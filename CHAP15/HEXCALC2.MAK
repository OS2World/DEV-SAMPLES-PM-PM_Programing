#------------------------
# HEXCALC2.MAK make file
#------------------------

hexcalc2.exe : hexcalc2.obj hexcalc2.def hexcalc.res
     $(PRGLINK) hexcalc2, hexcalc2, NUL, $(PRGLIB), hexcalc2
     rc hexcalc.res hexcalc2.exe

hexcalc2.obj: hexcalc2.c hexcalc.h
     $(PRGCC) hexcalc2.c

hexcalc.res : hexcalc.rc hexcalc.h
     $(PRGRC) hexcalc.rc
