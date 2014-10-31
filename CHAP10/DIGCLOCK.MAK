#------------------------
# DIGCLOCK.MAK make file 
#------------------------

digclock.exe : digclock.obj digclock.def
     $(PRGLINK) digclock, digclock, NUL, $(PRGLIB), digclock

digclock.obj : digclock.c
     $(PRGCC) digclock.c
