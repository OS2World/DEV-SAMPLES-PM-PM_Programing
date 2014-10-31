#------------------------
# OUTFONTS.MAK make file
#------------------------

outfonts.exe : outfonts.obj outfonts.def
     $(PRGLINK) outfonts, outfonts, NUL, $(PRGLIB), outfonts

outfonts.obj : outfonts.c
     $(PRGCC) outfonts.c
