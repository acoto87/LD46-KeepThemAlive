@echo off

REM Output path (relative to build bat file)
SET OutputPath=.\build\win64

REM Assets path (relative to output folder)
SET AssetsPath=..\..\assets

REM Include path (relative to output folder)
SET IncludePath=..\..\deps\include

REM Lib path (relative to output folder)
SET LibPath=..\..\deps\lib\win64

REM Compiler flags
REM SET ProfilerFlags="-pg"
SET OptimizeFlags="-O2"
REM SET AssemblyFlags="-g -Wa,-ahl"
SET CommonCompilerFlags=-std=c99 -Wall -Wno-misleading-indentation -x c -static-libgcc %ProfilerFlags% %OptimizeFlags% %AssemblyFlags% -I %IncludePath%
SET CommonLinkerFlags=-L %LibPath% -l glfw3 -l opengl32 -l ws2_32

ctime -begin build_timings.ctm

REM Create output path if doesn't exists
IF NOT EXIST %OutputPath% MKDIR %OutputPath%
PUSHD %OutputPath%

REM Empty the build folder
DEL /Q *

REM Compile the project
gcc %CommonCompilerFlags% ..\..\src\main.c ..\..\deps\include\glad\glad.c -o keep_them_alive.exe %CommonLinkerFlags%
if %ERRORLEVEL% == 0 (
    REM Copy assets
	COPY %AssetsPath%\* .\

	REM Copy dependencies
	COPY %LibPath%\glfw3.dll .\glfw3.dll
)

POPD

ctime -end build_timings.ctm

REM ctime -stats build_timings.ctm
REM ctime -csv build_timings.ctm
