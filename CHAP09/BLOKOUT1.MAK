#------------------------
# BLOKOUT1.MAK make file
#------------------------

blokout1.exe : blokout1.obj blokout1.def
     $(PRGLINK) blokout1, blokout1, NUL, $(PRGLIB), blokout1

blokout1.obj : blokout1.c
     $(PRGCC) blokout1.c
