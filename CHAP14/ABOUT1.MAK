#----------------------
# ABOUT1.MAK make file
#----------------------

about1.exe : about1.obj about1.def about1.res
     $(PRGLINK) about1, about1, NUL, $(PRGLIB), about1
     rc about1.res

about1.obj : about1.c about.h
     $(PRGCC) about1.c

about1.res : about1.rc about.h about.ico
     $(PRGRC) about1
