#-----------------------
# HDRTEST.MAK make file
#-----------------------

hdrtest.exe : hdrtest.obj hdrtest.def
     $(PRGLINK) hdrtest, hdrtest, NUL, $(PRGLIB), hdrtest

hdrtest.obj : hdrtest.c hdrlib.h
     $(PRGCC) hdrtest.c
