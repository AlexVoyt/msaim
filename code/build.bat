@echo off
setlocal ENABLEDELAYEDEXPANSION
REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
set OUT_DIR=..\Build
set OUT_EXE=win_main
set INCLUDES=/I"include" /I "%WindowsSdkDir%Include\um" /I "%WindowsSdkDir%Include\shared" /I "%DXSDK_DIR%Include"
if "%1%" == "full" (
    @set SOURCES=main.cpp include\imgui_impl_dx11.cpp include\imgui_impl_win32.cpp include\imgui*.cpp
    @set LIBS=/LIBPATH:"%DXSDK_DIR%/Lib/x86" d3d11.lib d3dcompiler.lib lib\sqlite3.lib
) else (
    @set SOURCES=main.cpp
    @set LIBS=..\Build\imgui*.obj /LIBPATH:"%DXSDK_DIR%/Lib/x86" d3d11.lib d3dcompiler.lib lib\sqlite3.lib
)
mkdir %OUT_DIR%
cl /nologo /Zi /MD /std:c++17 /EHsc %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%

