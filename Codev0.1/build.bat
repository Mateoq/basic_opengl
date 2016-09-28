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

cl -FC -Zi /EHsc /MP  ..\Code\main.cpp ..\Code\Shader.cpp ..\Code\Texture.cpp ..\Code\Camera.cpp ..\Code\Model.cpp ..\Code\Mesh.cpp -I%INCDIR% %LIBDIR%\glfw3dll.lib %LIBDIR%\assimpd.lib %LIBDIR%\glad.cpp user32.lib gdi32.lib shell32.lib opengl32.lib  -link /NODEFAULTLIB:library /machine:%TARGET% -subsystem:CONSOLE
popd
