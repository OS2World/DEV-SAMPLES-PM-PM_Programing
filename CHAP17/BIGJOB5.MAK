#-----------------------
# BIGJOB5.MAK make file
#-----------------------

bigjob5.exe : bigjob5.obj bigjobmt.obj bigjob5.def bigjob.res
     $(PRGLINK) bigjob5 bigjobmt, bigjob5, NUL, $(PRGLIBMT), bigjob5
     rc bigjob.res bigjob5.exe

bigjob5.obj : bigjob5.c bigjob.h
     $(PRGCCMT)bigjob5.obj bigjob5.c

bigjobmt.obj : bigjob.c
     $(PRGCCMT)bigjobmt.obj bigjob.c

bigjob.res : bigjob.rc bigjob.h
     $(PRGRC) bigjob
