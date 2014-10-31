#------------------------
# PATTERNS.MAK make file
#------------------------

patterns.exe : patterns.obj patterns.def
     $(PRGLINK) patterns, patterns, NUL, $(PRGLIB), patterns

patterns.obj : patterns.c
     $(PRGCC) patterns.c
