#-------------------
# WEB.MAK make file
#-------------------

web.exe : web.obj web.def
     $(PRGLINK) web, web, NUL, $(PRGLIB), web

web.obj : web.c
     $(PRGCC) web.c
