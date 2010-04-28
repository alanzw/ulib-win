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

g++ -c %SRCDIR%/msw/hwApp.cpp -o hy_window.o -I"%INCDIR%" -DHW_MSW -DHW_GUI
g++ -c %SRCDIR%/hwApp.cpp -o hy_hwApp.o -I"%INCDIR%" -DHW_MSW -DHW_GUI
g++ -c window.cpp -I"%INCDIR%" -DHW_MSW -DHW_GUI

g++ -o window.exe *.o -mwindows

echo ==========================

REM g++ -c %SRCDIR%/hwApp.cpp -o dll_hwApp.o -I"%INCDIR%" -DHW_CONSOLE -DHW_MSW -DBUILD_DLL
REM g++ -shared -o hwApp.dll dll_hwApp.o

echo ==========================
echo End building
echo ==========================

@pause
