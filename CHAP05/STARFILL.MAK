#------------------------
# STARFILL.MAK make file
#------------------------

starfill.exe : starfill.obj starfill.def
     $(PRGLINK) starfill, starfill, NUL, $(PRGLIB), starfill

starfill.obj : starfill.c
     $(PRGCC) starfill.c
