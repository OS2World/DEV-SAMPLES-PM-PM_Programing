#-----------------------
# PATTDLG.MAK make file
#-----------------------

pattdlg.exe : pattdlg.obj pattdlg.def pattdlg.res
     $(PRGLINK) pattdlg, pattdlg, NUL, $(PRGLIB), pattdlg
     rc pattdlg.res

pattdlg.obj : pattdlg.c pattdlg.h
     $(PRGCC) pattdlg.c

pattdlg.res : pattdlg.rc pattdlg.h
     $(PRGRC) pattdlg
