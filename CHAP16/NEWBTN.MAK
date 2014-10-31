#----------------------
# NEWBTN.MAK make file
#----------------------

newbtn.dll : newbtndl.obj newbtn.def
     $(DLLLINK) newbtndl, newbtn.dll, NUL, $(DLLLIB), newbtn
     implib newbtn.lib newbtn.def

newbtndl.obj : newbtn.c newbtn.h
     $(DLLCC)newbtndl.obj newbtn.c
