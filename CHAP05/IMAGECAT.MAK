#------------------------
# IMAGECAT.MAK make file
#------------------------

imagecat.exe : imagecat.obj imagecat.def
     $(PRGLINK) imagecat, imagecat, NUL, $(PRGLIB), imagecat

imagecat.obj : imagecat.c
     $(PRGCC) imagecat.c
