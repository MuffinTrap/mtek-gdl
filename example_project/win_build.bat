@echo off
REM This bat will build the windows native version of the
REM example project
pushd build
REM set output name to example.exe
cl ..\main.cpp
REM copy exe back to main dir
popd
