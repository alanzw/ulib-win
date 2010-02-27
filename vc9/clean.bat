@echo off

rem VCVARS32 Setup
call D:\Progs\VS9\VC\VCVARSALL.bat

devenv ulib_win.sln /Clean

rm *.plg *.opt *.ncb *.user *.suo

rm -rvf Debug Release

rm -vf Upgrade*

rm -rvf _Upgrade*

@pause

@echo on
