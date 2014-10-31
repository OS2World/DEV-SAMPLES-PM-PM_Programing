#--------------------
# WELC.MAK make file
#--------------------

welc.exe : welc.obj welc.def
     $(PRGLINK) welc, welc, NUL, $(PRGLIB), welc

welc.obj : welc.c
     $(PRGCC) welc.c
