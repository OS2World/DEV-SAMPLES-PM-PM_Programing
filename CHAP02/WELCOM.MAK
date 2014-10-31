#----------------------
# WELCOM.MAK make file
#----------------------

welcom.exe : welcom.obj welcom.def
     $(PRGLINK) welcom, welcom, NUL, $(PRGLIB), welcom

welcom.obj : welcom.c
     $(PRGCC) welcom.c
