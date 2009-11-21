@echo off

rem filename : build.bat
rem author   : huys
rem updated  : 2008.11.21.
rem version  : 0.1

echo **************************
echo ** hyWidget 0.1
echo ** author : huys
echo ** build  : 200901
echo **************************
echo -------
echo -------

echo ==========================
echo Start building
echo ==========================

set INCDIR=../../inc
set SRCDIR=../../src

g++ -c %SRCDIR%/msw/hwApp.cpp -o hy_console.o -I"%INCDIR%" -DHW_CONSOLE
g++ -c %SRCDIR%/hwApp.cpp -o hy_hwApp.o -I"%INCDIR%" -DHW_CONSOLE
g++ -c console.cpp -I"%INCDIR%" -DHW_CONSOLE

g++ -o console.exe *.o

echo ==========================

REM g++ -c %SRCDIR%/hwApp.cpp -o dll_hwApp.o -I"%INCDIR%" -DHW_CONSOLE -DHW_MSW -DBUILD_DLL
REM g++ -shared -o hwApp.dll dll_hwApp.o

echo ==========================
echo End building
echo ==========================

@pause
