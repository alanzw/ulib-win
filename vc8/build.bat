@echo off

rem VCVARS32 Setup
call D:\Progs\VS8\VC\VCVARSALL.bat

devenv ulib_win.sln /build "Debug"

@pause
