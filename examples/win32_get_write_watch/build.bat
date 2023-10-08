@echo off

IF NOT EXIST build mkdir build

pushd build

cl ..\write_watch.cpp /nologo /Od /Zi /wd4312

popd


