#------------------------
# BUTTONS1.MAK make file
#------------------------

buttons1.exe : buttons1.obj buttons1.def
     $(PRGLINK) buttons1, buttons1, NUL, $(PRGLIB), buttons1

buttons1.obj : buttons1.c
     $(PRGCC) buttons1.c
