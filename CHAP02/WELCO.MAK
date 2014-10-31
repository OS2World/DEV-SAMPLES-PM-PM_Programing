#---------------------
# WELCO.MAK make file
#---------------------

welco.exe : welco.obj welco.def
     $(PRGLINK) welco, welco, NUL, $(PRGLIB), welco

welco.obj : welco.c
     $(PRGCC) welco.c
