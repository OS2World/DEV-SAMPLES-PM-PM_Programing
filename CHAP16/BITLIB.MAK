#----------------------
# BITLIB.MAK make file
#----------------------

bitlib.dll : bitlib.obj bitlib.def bitlib.res
     $(DLLLINK) bitlib, bitlib.dll, NUL, $(DLLLIB), bitlib
     rc bitlib.res bitlib.dll

bitlib.obj : bitlib.c
     $(DLLCC)bitlib.obj bitlib.c

bitlib.res : bitlib.rc bitmap1.bmp bitmap2.bmp bitmap3.bmp \
                       bitmap4.bmp bitmap5.bmp bitmap6.bmp \
                       bitmap7.bmp bitmap8.bmp bitmap9.bmp
     $(DLLRC) bitlib
