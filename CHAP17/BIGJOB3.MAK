#-----------------------
# BIGJOB3.MAK make file
#-----------------------

bigjob3.exe : bigjob3.obj bigjob.obj bigjob3.def bigjob.res
     $(PRGLINK) bigjob3 bigjob, bigjob3, NUL, $(PRGLIB), bigjob3
     rc bigjob.res bigjob3.exe

bigjob3.obj : bigjob3.c bigjob.h
     $(PRGCC) bigjob3.c

bigjob.obj : bigjob.c
     $(PRGCC) bigjob.c

bigjob.res : bigjob.rc bigjob.h
     $(PRGRC) bigjob
