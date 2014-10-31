#---------------------
# RULER.MAK make file
#---------------------

ruler.exe : ruler.obj ruler.def
     $(PRGLINK) ruler, ruler, NUL, $(PRGLIB), ruler

ruler.obj : ruler.c
     $(PRGCC) ruler.c
