@echo off

rem VCVARS32 Setup
call D:\Progs\VS8\VC\VCVARSALL.bat

devenv ulib_win.sln /Clean

rm *.plg *.opt *.ncb *.user *.suo

rm -rvf Debug Release

@pause

@echo on
