@echo off

rem VCVARS32 Setup
call D:\GS\VC6\VC98\Bin\VCVARS32.bat

rem msdev libuwin_static.dsp /MAKE /CLEAN
msdev libuwin_static.dsp /MAKE

rm Debug\*.obj

msdev libuwin_dll.dsp /MAKE

msdev dlg.dsp /MAKE

msdev winx.dsp /MAKE

rem devenv libuwin_static.dsp /build "Debug"

@pause

@echo on
