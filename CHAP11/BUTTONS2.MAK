#------------------------
# BUTTONS2.MAK make file
#------------------------

buttons2.exe : buttons2.obj newbtn.obj buttons2.def
     $(PRGLINK) buttons2 newbtn, buttons2, NUL, $(PRGLIB), buttons2

buttons2.obj : buttons2.c
     $(PRGCC) buttons2.c

newbtn.obj : newbtn.c
     $(PRGCC) newbtn.c
