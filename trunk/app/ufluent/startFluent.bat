@echo off

set FLUENT_INC=D:\GS\Fluent.Inc

set FLUENT_ARCH=ntx86

set FLUENT_REVISION=26

set SEEMPATH=%FLUENT_INC%\ntbin\ntx86;%FLUENT_INC%\fluent6.3.26\lib;%FLUENT_INC%\fluent6.3.26\client\lib;%FLUENT_INC%\fluent6.3.26\cortex\lib

set USER_LOCAL=u:

set FLUENT_HELP=%FLUENT_INC%\nthelp\main.hlp

set PATH=%FLUENT_INC%\ntbin\ntx86;%FLUENT_INC%\ntbin\ntx86\opengl;%FLUENT_INC%\fluent6.3.26\vki\ntx86;%FLUENT_INC%\license\ntx86;%PATH%

set FLUENT_PROG=fluent

set FLUENT_PROG_DIR=%FLUENT_INC%\fluent6.3.26

set FLUENT_ADDONS=%FLUENT_INC%\addons

REM cd %FLUENT_INC%\ntbin\ntx86

set LM_LICENSE_FILE=%FLUENT_INC%\license\license.dat

%FLUENT_INC%\fluent6.3.26\ntx86\3d\fl6326s.exe

REM D:\GS\OllyDbg\OllyICE.exe

REM D:\Progs\IDA\idag.exe
