@echo off

rem VCVARS32 Setup
call D:\Progs\VS10\VC\VCVARSALL.bat

devenv ulib_win.sln /build "Debug"

@pause
