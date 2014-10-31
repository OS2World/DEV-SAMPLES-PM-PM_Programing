#----------------------
# COLORS.MAK make file
#----------------------

colors.exe : colors.obj colors.def
     $(PRGLINK) colors, colors, NUL, $(PRGLIB), colors

colors.obj : colors.c
     $(PRGCC) colors.c
