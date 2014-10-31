#------------------------
# LOADBMP2.MAK make file
#------------------------

loadbmp2.exe : loadbmp2.obj loadbmp2.def loadbmp.res
     $(PRGLINK) loadbmp2, loadbmp2, NUL, $(PRGLIB), loadbmp2
     rc loadbmp.res loadbmp2.exe

loadbmp2.obj : loadbmp2.c loadbmp.h
     $(PRGCC) loadbmp2.c

loadbmp.res : loadbmp.rc loadbmp.h loadbmp.bmp
     $(PRGRC) loadbmp
