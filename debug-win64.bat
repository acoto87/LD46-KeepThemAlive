@echo off

REM Output path (relative to build bat file)
SET OutputPath=.\build\win64

PUSHD %OutputPath%

CALL gdb keep_them_alive.exe

POPD
