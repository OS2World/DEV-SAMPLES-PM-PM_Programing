#---------------------
# FONTS.MAK make file
#---------------------

fonts.exe : fonts.obj easyfont.obj fonts.def
     $(PRGLINK) fonts easyfont, fonts, NUL, $(PRGLIB), fonts

fonts.obj : fonts.c easyfont.h
     $(PRGCC) fonts.c

easyfont.obj : easyfont.c
     $(PRGCC) easyfont.c
