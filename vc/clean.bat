@echo off

rem VCVARS32 Setup
call D:\GS\VC6\VC98\Bin\VCVARS32.bat

msdev libuwin_static.dsp /MAKE /CLEAN

msdev libuwin_dll.dsp /MAKE /CLEAN

msdev dlg.dsp /MAKE /CLEAN

msdev winx.dsp /MAKE /CLEAN

msdev opengl.dsp /MAKE /CLEAN

msdev conf.dsp /MAKE /CLEAN

msdev chart.dsp /MAKE /CLEAN

msdev adt.dsp /MAKE /CLEAN

msdev toolbar.dsp /MAKE /CLEAN

msdev edit.dsp /MAKE /CLEAN

msdev rebar.dsp /MAKE /CLEAN

msdev combobox.dsp /MAKE /CLEAN

msdev comboboxex.dsp /MAKE /CLEAN

msdev ini.dsp /MAKE /CLEAN

rem devenv libuwin_static.dsp /build "Debug"

rm *.plg *.opt *.ncb

rm -rvf Debug Release

@pause

@echo on
