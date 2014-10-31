#------------------------
# COLORSCR.MAK make file
#------------------------

colorscr.exe : colorscr.obj colorscr.def
     $(PRGLINK) colorscr, colorscr, NUL, $(PRGLIB), colorscr

colorscr.obj : colorscr.c
     $(PRGCC) colorscr.c
