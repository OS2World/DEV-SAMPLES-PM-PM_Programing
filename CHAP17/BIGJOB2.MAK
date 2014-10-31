#-----------------------
# BIGJOB2.MAK make file
#-----------------------

bigjob2.exe : bigjob2.obj bigjob.obj bigjob2.def bigjob.res
     $(PRGLINK) bigjob2 bigjob, bigjob2, NUL, $(PRGLIB), bigjob2
     rc bigjob.res bigjob2.exe

bigjob2.obj : bigjob2.c bigjob.h
     $(PRGCC) bigjob2.c

bigjob.obj : bigjob.c
     $(PRGCC) bigjob.c

bigjob.res : bigjob.rc bigjob.h
     $(PRGRC) bigjob
