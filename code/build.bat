@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl -nologo ..\code\kninja.cpp
popd
