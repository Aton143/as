@echo off

IF NOT EXIST build mkdir build

pushd build

cl ..\cpp.cpp /nologo /Od /Zi /wd4312 /EHa

popd


