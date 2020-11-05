@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl -nologo /FC ..\code\kninja.cpp /link user32.lib D2d1.lib Dwrite.lib
popd
