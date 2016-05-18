@echo off

if not exist ..\Build  mkdir ..\Build
pushd ..\Build

SETLOCAL

set TARGET=x64

:: All dependecies
set DEPS=..\Dependencies

:: Libraries and Includes
set LIBDIR=%DEPS%\lib
set INCDIR=%DEPS%\include

set SDL2_LIBDIR=%LIBDIR%\%TARGET%

if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
)

robocopy %LIBDIR% . *.dll

cl -FC -Zi /EHsc  ..\Code\main.cpp -I%INCDIR% %SDL2_LIBDIR%\SDL2.lib %SDL2_LIBDIR%\SDL2main.lib %LIBDIR%\glad.c  -link /machine:%TARGET% -subsystem:CONSOLE
popd
