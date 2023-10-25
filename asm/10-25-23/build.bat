@echo off

IF NOT EXIST build mkdir build
pushd build

ml64 ..\library.asm /nologo /WX /W3 /Zi /c /Folibrary.obj
REM lib /nologo /machine:X64 /out:library.lib /LTCG /WX library.obj

cl ..\main.cpp /c /nologo /FC /Zi /GS- /Ob2 /GL
link main.obj library.obj /LTCG /NOLOGO

popd
