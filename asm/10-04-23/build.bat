@echo off


IF NOT EXIST build mkdir build

pushd build

del *.pdb > NUL 2> NUL

ml64 /nologo /WX /W3 /Zi /c /Foasm.obj ..\main.s
cl ..\main.cpp /nologo /Oi /FC /Z7 /WX /W4 /MT /Gm- /GR- /GS- /EHa- /EHc- /EHsc- /link asm.obj /incremental:no /opt:ref /DEBUG:FULL

popd
