@echo off

rem filename : build.bat
rem author   : huys
rem updated  : 2008.11.21.
rem version  : 0.1

echo **************************
echo ** ulib-win 0.1
echo ** author : huys
echo ** build  : 200901
echo **************************
echo -------
echo -------

echo ==========================
echo Start building
echo ==========================


set DOXY_DIR=D:\Progs\doxygen\bin
set TEX_DIR=H:\bin\win32
set PERL_DIR=D:\Progs\Perl\bin

set PATH=%DOXY_DIR%;%TEX_DIR%;%PERL_DIR%;%PATH%

rem
make UDOC


echo ==========================
echo End building
echo ==========================

@pause
