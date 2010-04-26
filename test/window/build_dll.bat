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

g++ -fPIC -c %SRCDIR%/msw/hwApp.cpp -o hy_window_d.o -I"%INCDIR%" -DHW_MSW -DBUILD_DLL
g++ -fPIC -c %SRCDIR%/hwApp.cpp -o hy_hwApp_d.o -I"%INCDIR%" -DHW_MSW -DBUILD_DLL

g++ -shared -o hw.dll *_d.o -Wl,--output-def,libhw.def,--out-implib,libhw.dll.a

g++ -c window.cpp -I"%INCDIR%" -DHW_MSW -DUSE_DLL

g++ -o window.exe window.o -L. -lhw

echo ==========================

REM g++ -c %SRCDIR%/hwApp.cpp -o dll_hwApp.o -I"%INCDIR%" -DHW_CONSOLE -DHW_MSW -DBUILD_DLL
REM g++ -shared -o hwApp.dll dll_hwApp.o

echo ==========================
echo End building
echo ==========================

@pause
