@echo off

@set INCLUDES=/I..\..\..\common

IF NOT EXIST build mkdir build

pushd build

cl ..\debug_loop.cpp /nologo /Od /Zi /wd4312 %INCLUDES% /EHsc-
cl ..\loop.cpp /nologo /Od /Zi /wd4312

popd


