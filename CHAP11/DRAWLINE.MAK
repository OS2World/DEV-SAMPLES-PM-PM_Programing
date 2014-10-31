#------------------------
# DRAWLINE.MAK make file
#------------------------

drawline.exe : drawline.obj drawline.def
     $(PRGLINK) drawline, drawline, NUL, $(PRGLIB), drawline

drawline.obj : drawline.c
     $(PRGCC) drawline.c
