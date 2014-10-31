#------------------------
# BUTTONS3.MAK make file
#------------------------

buttons3.exe : buttons3.obj buttons3.def newbtn.lib
     $(PRGLINK) buttons3, buttons3, NUL, $(PRGLIB) newbtn, buttons3

buttons3.obj : buttons3.c newbtn.h
     $(PRGCC) buttons3.c

newbtn.lib : newbtn.c newbtn.h newbtn.def
     nmake newbtn.mak
