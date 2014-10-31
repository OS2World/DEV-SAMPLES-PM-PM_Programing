#----------------------
# BLOWUP.MAK make file
#----------------------

blowup.exe : blowup.obj blowup.def blowup.res
     $(PRGLINK) blowup, blowup, NUL, $(PRGLIB), blowup
     rc blowup.res

blowup.obj : blowup.c blowup.h
     $(PRGCC) blowup.c

blowup.res : blowup.rc blowup.h
     $(PRGRC) blowup
