#----------------------
# HDRLIB.MAK make file
#----------------------

hdrlib.dll : hdrlib.obj hdrlib.def
     $(DLLLINK) hdrlib, hdrlib.dll, NUL, $(DLLLIB), hdrlib

hdrlib.obj : hdrlib.c hdrlib.h
     $(DLLCC)hdrlib.obj hdrlib.c
