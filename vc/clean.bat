@echo off

rem VCVARS32 Setup
call D:\GS\VC6\VC98\Bin\VCVARS32.bat

rem devenv libuwin_static.dsp /build "Debug"

rm *.plg *.opt *.ncb

rm -rvf Debug Release

@pause
