#----------------------
# ABOUT2.MAK make file
#----------------------

about2.exe : about2.obj newbtn.obj about2.def about2.res
     $(PRGLINK) about2 newbtn, about2, NUL, $(PRGLIB), about2
     rc about2.res

about2.obj : about2.c about.h
     $(PRGCC) about2.c

newbtn.obj : newbtn.c
     $(PRGCC) newbtn.c

about2.res : about2.rc about.h about.ico
     $(PRGRC) about2
