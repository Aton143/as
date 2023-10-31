@echo off

@set INCLUDES=/I..\..\..\common

IF NOT EXIST build mkdir build

pushd build

cl ..\page_fault.cpp /nologo /Od /Zi /wd4312 %INCLUDES% /EHsc- /EHa-

popd


