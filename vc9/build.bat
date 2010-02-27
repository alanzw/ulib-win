@echo off

rem VCVARS32 Setup
call D:\Progs\VS9\VC\VCVARSALL.bat

devenv ulib_win.sln /build "Debug"

@pause
