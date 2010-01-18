@echo off

rem VCVARS32 Setup
call D:\Progs\VS10\VC\VCVARSALL.bat

devenv ulib_win.sln /Clean

rm *.plg *.opt *.ncb *.user *.suo *.sdf *.opensdf *.log

rm -rvf Debug Release ipch

@pause

@echo on
