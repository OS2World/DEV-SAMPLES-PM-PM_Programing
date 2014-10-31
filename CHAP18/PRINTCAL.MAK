#--------------------
# PRINTCAL make file
#--------------------

printcal.exe : printcal.obj printdc.obj printcal.def printcal.res
     $(PRGLINK) printcal printdc, printcal, NUL, $(PRGLIBMT), printcal
     rc printcal.res

printcal.obj : printcal.c
     $(PRGCCMT)printcal.obj printcal.c

printdc.obj : printdc.c
     $(PRGCCMT)printdc.obj printdc.c

printcal.res : printcal.rc
     $(PRGRC) printcal
