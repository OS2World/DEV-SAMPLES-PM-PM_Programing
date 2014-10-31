#-----------------------
# BIGJOB4.MAK make file
#-----------------------

bigjob4.exe : bigjob4.obj bigjobmt.obj bigjob4.def bigjob.res
     $(PRGLINK) bigjob4 bigjobmt, bigjob4, NUL, $(PRGLIBMT), bigjob4
     rc bigjob.res bigjob4.exe

bigjob4.obj : bigjob4.c bigjob.h
     $(PRGCCMT)bigjob4.obj bigjob4.c

bigjobmt.obj : bigjob.c
     $(PRGCCMT)bigjobmt.obj bigjob.c

bigjob.res : bigjob.rc bigjob.h
     $(PRGRC) bigjob
