#-------------------
# WEL.MAK make file
#-------------------

wel.exe : wel.obj wel.def
     $(PRGLINK) wel, wel, NUL, $(PRGLIB), wel

wel.obj : wel.c
     $(PRGCC) wel.c
