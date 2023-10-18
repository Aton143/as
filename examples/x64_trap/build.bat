@echo off

@set INCLUDES=/I..\..\..\common

IF NOT EXIST build mkdir build

pushd build

cl ..\x64_trap.cpp /nologo /Od /Zi /wd4312 %INCLUDES% /EHa

popd


