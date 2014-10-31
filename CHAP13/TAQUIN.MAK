#----------------------
# TAQUIN.MAK make file
#----------------------

taquin.exe : taquin.obj taquin.def taquin.res
     $(PRGLINK) taquin, taquin, NUL, $(PRGLIB), taquin
     rc taquin.res

taquin.obj : taquin.c taquin.h
     $(PRGCC) taquin.c

taquin.res : taquin.rc taquin.h taquin.ico
     $(PRGRC) taquin
