#---------------------
# PATHS.MAK make file
#---------------------

paths.exe : paths.obj paths.def
     $(PRGLINK) paths, paths, NUL, $(PRGLIB), paths

paths.obj : paths.c
     $(PRGCC) paths.c
