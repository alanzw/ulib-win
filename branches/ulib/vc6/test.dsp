# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "test - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../debug"
# PROP Intermediate_Dir "../debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../inc" /I "../sample/test" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/test_debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\sample\test\ChartDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\mypicture.cpp
# End Source File
# Begin Source File

SOURCE=..\sample\test\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\sample\test\test.cpp
# End Source File
# Begin Source File

SOURCE=..\sample\test\test.rc
# End Source File
# Begin Source File

SOURCE=..\sample\test\testDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uButton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uChart.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ugdi.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uLedCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uLink.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uSplash.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uText.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uToolbar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uTooltip.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\sample\test\ChartDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\colors.h
# End Source File
# Begin Source File

SOURCE=..\inc\mypicture.h
# End Source File
# Begin Source File

SOURCE=..\sample\test\resource.h
# End Source File
# Begin Source File

SOURCE=..\sample\test\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\sample\test\test.h
# End Source File
# Begin Source File

SOURCE=..\sample\test\testDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\uButton.h
# End Source File
# Begin Source File

SOURCE=..\inc\uChart.h
# End Source File
# Begin Source File

SOURCE=..\inc\ugdi.h
# End Source File
# Begin Source File

SOURCE=..\inc\uLedCtrl.h
# End Source File
# Begin Source File

SOURCE=..\inc\uLink.h
# End Source File
# Begin Source File

SOURCE=..\inc\uListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\inc\uMenu.h
# End Source File
# Begin Source File

SOURCE=..\inc\uSplash.h
# End Source File
# Begin Source File

SOURCE=..\inc\uText.h
# End Source File
# Begin Source File

SOURCE=..\inc\uToolbar.h
# End Source File
# Begin Source File

SOURCE=..\inc\uTooltip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\sample\test\res\straight.bmp
# End Source File
# Begin Source File

SOURCE=..\sample\test\res\test.ico
# End Source File
# Begin Source File

SOURCE=..\sample\test\res\toolbar1.bmp
# End Source File
# End Group
# End Target
# End Project
