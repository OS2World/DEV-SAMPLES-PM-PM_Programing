#-----------------------
# FREEMEM.MAK make file 
#-----------------------

freemem.exe : freemem.obj freemem.def
     $(PRGLINK) freemem, freemem, NUL, $(PRGLIB), freemem

freemem.obj : freemem.c
     $(PRGCC) freemem.c
