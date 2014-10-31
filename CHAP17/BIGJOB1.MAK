#-----------------------
# BIGJOB1.MAK make file
#-----------------------

bigjob1.exe : bigjob1.obj bigjob.obj bigjob1.def bigjob.res
     $(PRGLINK) bigjob1 bigjob, bigjob1, NUL, $(PRGLIB), bigjob1
     rc bigjob.res bigjob1.exe

bigjob1.obj : bigjob1.c bigjob.h
     $(PRGCC) bigjob1.c

bigjob.obj : bigjob.c
     $(PRGCC) bigjob.c

bigjob.res : bigjob.rc bigjob.h
     $(PRGRC) bigjob
