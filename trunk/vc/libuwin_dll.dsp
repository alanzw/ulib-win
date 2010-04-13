# Microsoft Developer Studio Project File - Name="libuwin_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libuwin_dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libuwin_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libuwin_dll.mak" CFG="libuwin_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libuwin_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libuwin_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libuwin_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release/libuwin_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBUWIN_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../inc" /I "." /I "./fix" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBUWIN_DLL_EXPORTS" /D "BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /i "../ico" /i "../res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib opengl32.lib glu32.lib /nologo /dll /machine:I386 /out:"Release/libuwin_release.vc6.dll" /implib:"Release/libuwin_release.vc6.dll.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libuwin_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug/libuwin_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBUWIN_DLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../inc" /I "." /I "./fix" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBUWIN_DLL_EXPORTS" /D "BUILD_DLL" /D "HU_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /i "../ico" /i "../res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /out:"Debug/libuwin_debug.vc6.dll" /implib:"Debug/libuwin_debug.vc6.dll.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libuwin_dll - Win32 Release"
# Name "libuwin_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\ubasewindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ubitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ubutton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ucombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ucomboboxex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ucommondialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uconf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ucontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udebug.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udialogx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udibapi.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udllman.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ufile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ufont.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ugdi.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ugldialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uglut.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uicon.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uimagelist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uini.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ulib.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ulistbox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ulistview.cpp
# End Source File
# Begin Source File

SOURCE=..\src\umenu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\umsg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uobject.cpp
# End Source File
# Begin Source File

SOURCE=..\src\upicture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uprinter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uprocessman.cpp
# End Source File
# Begin Source File

SOURCE=..\src\urebar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uregion.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uregkey.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ustatic.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ustatusbar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\usync.cpp
# End Source File
# Begin Source File

SOURCE=..\src\usys.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utabctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utoolbar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uwinapp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uwindowx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\ubasewindow.h
# End Source File
# Begin Source File

SOURCE=..\inc\ubitmap.h
# End Source File
# Begin Source File

SOURCE=..\inc\ubutton.h
# End Source File
# Begin Source File

SOURCE=..\inc\ucombobox.h
# End Source File
# Begin Source File

SOURCE=..\inc\ucomboboxex.h
# End Source File
# Begin Source File

SOURCE=..\inc\ucommondialog.h
# End Source File
# Begin Source File

SOURCE=..\inc\uconf.h
# End Source File
# Begin Source File

SOURCE=..\inc\ucontrol.h
# End Source File
# Begin Source File

SOURCE=..\inc\udc.h
# End Source File
# Begin Source File

SOURCE=..\inc\udebug.h
# End Source File
# Begin Source File

SOURCE=..\inc\udialogx.h
# End Source File
# Begin Source File

SOURCE=..\inc\udibapi.h
# End Source File
# Begin Source File

SOURCE=..\inc\udllman.h
# End Source File
# Begin Source File

SOURCE=..\inc\uedit.h
# End Source File
# Begin Source File

SOURCE=..\inc\ufile.h
# End Source File
# Begin Source File

SOURCE=..\inc\ufont.h
# End Source File
# Begin Source File

SOURCE=..\inc\ugdi.h
# End Source File
# Begin Source File

SOURCE=..\inc\ugldialog.h
# End Source File
# Begin Source File

SOURCE=..\inc\uglut.h
# End Source File
# Begin Source File

SOURCE=..\inc\uicon.h
# End Source File
# Begin Source File

SOURCE=..\inc\uimagelist.h
# End Source File
# Begin Source File

SOURCE=..\inc\uini.h
# End Source File
# Begin Source File

SOURCE=..\inc\ulib.h
# End Source File
# Begin Source File

SOURCE=..\inc\ulistbox.h
# End Source File
# Begin Source File

SOURCE=..\inc\ulistview.h
# End Source File
# Begin Source File

SOURCE=..\inc\umenu.h
# End Source File
# Begin Source File

SOURCE=..\inc\umsg.h
# End Source File
# Begin Source File

SOURCE=..\inc\uobject.h
# End Source File
# Begin Source File

SOURCE=..\inc\upicture.h
# End Source File
# Begin Source File

SOURCE=..\inc\uprinter.h
# End Source File
# Begin Source File

SOURCE=..\inc\uprocessman.h
# End Source File
# Begin Source File

SOURCE=..\inc\urebar.h
# End Source File
# Begin Source File

SOURCE=..\inc\uregion.h
# End Source File
# Begin Source File

SOURCE=..\inc\uregkey.h
# End Source File
# Begin Source File

SOURCE=..\inc\ustatic.h
# End Source File
# Begin Source File

SOURCE=..\inc\ustatusbar.h
# End Source File
# Begin Source File

SOURCE=..\inc\usync.h
# End Source File
# Begin Source File

SOURCE=..\inc\usys.h
# End Source File
# Begin Source File

SOURCE=..\inc\utabctrl.h
# End Source File
# Begin Source File

SOURCE=..\inc\utoolbar.h
# End Source File
# Begin Source File

SOURCE=..\inc\utooltip.h
# End Source File
# Begin Source File

SOURCE=..\inc\uwarnings.h
# End Source File
# Begin Source File

SOURCE=..\inc\uwinapp.h
# End Source File
# Begin Source File

SOURCE=..\inc\uwindowx.h
# End Source File
# Begin Source File

SOURCE=..\inc\uworkaround.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\res\u.rc

!IF  "$(CFG)" == "libuwin_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "libuwin_dll - Win32 Debug"

# ADD BASE RSC /l 0x804 /i "\ulib-win\res" /i "\DEV\ulib-win\res"
# SUBTRACT BASE RSC /i "../ico" /i "../res"
# ADD RSC /l 0x804 /i "..\res" /i "..\ico" /i "\ulib-win\res"
# SUBTRACT RSC /i "../ico" /i "../res"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
