#----------------------
# BRICKS.MAK make file
#----------------------

bricks.exe : bricks.obj bricks.def
     $(PRGLINK) bricks, bricks, NUL, $(PRGLIB), bricks

bricks.obj : bricks.c
     $(PRGCC) bricks.c
