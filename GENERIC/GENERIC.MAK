generic.res: generic.rc generic.h
   rc /r generic.rc
   
generic.obj: generic.c generic.h
   cl /c /Gsw /Oas /Zpe generic.c

generic.exe: generic.obj generic.def
   link /nod generic, , , slibcew libw, generic.def
   rc generic.res
   
generic.exe: generic.res
   rc generic.res
