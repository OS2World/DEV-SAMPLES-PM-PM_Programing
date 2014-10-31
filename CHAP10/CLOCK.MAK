#---------------------
# CLOCK.MAK make file
#---------------------

clock.exe : clock.obj clock.def
     $(PRGLINK) clock, clock, NUL, $(PRGLIB), clock

clock.obj : clock.c
     $(PRGCC) clock.c
