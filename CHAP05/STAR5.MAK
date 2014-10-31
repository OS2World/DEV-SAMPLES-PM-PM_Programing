#---------------------
# STAR5.MAK make file
#---------------------

star5.exe : star5.obj star5.def
     $(PRGLINK) star5, star5, NUL, $(PRGLIB), star5

star5.obj : star5.c
     $(PRGCC) star5.c
