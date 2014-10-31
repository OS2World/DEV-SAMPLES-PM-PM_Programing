#------------------------
# BLOKOUT2.MAK make file
#------------------------

blokout2.exe : blokout2.obj blokout2.def
     $(PRGLINK) blokout2, blokout2, NUL, $(PRGLIB), blokout2

blokout2.obj : blokout2.c
     $(PRGCC) blokout2.c
