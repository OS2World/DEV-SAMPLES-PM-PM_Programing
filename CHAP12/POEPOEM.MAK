#-----------------------
# POEPOEM.MAK make file
#-----------------------

poepoem.exe : poepoem.obj poepoem.def poepoem.res
     $(PRGLINK) poepoem, poepoem, NUL, $(PRGLIB), poepoem
     rc poepoem.res

poepoem.obj : poepoem.c poepoem.h
     $(PRGCC) poepoem.c

poepoem.res : poepoem.rc poepoem.ico poepoem.asc poepoem.h
     $(PRGRC) poepoem
