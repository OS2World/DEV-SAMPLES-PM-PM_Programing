#------------------------
# RESOURCE.MAK make file
#------------------------

resource.exe : resource.obj resource.def resource.res
     $(PRGLINK) resource, resource, NUL, $(PRGLIB), resource
     rc resource.res

resource.obj : resource.c resource.h
     $(PRGCC) resource.c

resource.res : resource.rc resource.h resource.ico resource.ptr
     $(PRGRC) resource
