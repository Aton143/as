@echo off

@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
@set OUT_EXE=sockets
@set INCLUDES=/ID:\imgui /ID:\imgui\backends

@set SOURCES=..\main.cpp D:\imgui\backends\imgui_impl_dx11.cpp D:\imgui\backends\imgui_impl_win32.cpp
@set SOURCES=%SOURCES% D:\imgui\imgui.cpp D:\imgui\imgui_draw.cpp D:\imgui\imgui_tables.cpp D:\imgui\imgui_widgets.cpp

@set LIBS=d3d11.lib d3dcompiler.lib

IF NOT EXIST build mkdir build
pushd build
cl /nologo /Zi /MD /utf-8 %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_EXE%.exe /link %LIBS%
popd build

