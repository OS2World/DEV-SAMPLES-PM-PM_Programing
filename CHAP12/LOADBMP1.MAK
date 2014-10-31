#------------------------
# LOADBMP1.MAK make file
#------------------------

loadbmp1.exe : loadbmp1.obj loadbmp1.def loadbmp.res
     $(PRGLINK) loadbmp1, loadbmp1, NUL, $(PRGLIB), loadbmp1
     rc loadbmp.res loadbmp1.exe

loadbmp1.obj : loadbmp1.c loadbmp.h
     $(PRGCC) loadbmp1.c

loadbmp.res : loadbmp.rc loadbmp.h loadbmp.bmp
     $(PRGRC) loadbmp
