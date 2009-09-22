# Microsoft Developer Studio Project File - Name="libuwin_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libuwin_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libuwin_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libuwin_static.mak" CFG="libuwin_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libuwin_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libuwin_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libuwin_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "." /I "../inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "BUILD_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\libuwin_release.vc6.lib"

!ELSEIF  "$(CFG)" == "libuwin_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "../inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "BUILD_STATIC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\libuwin_debug.vc6.lib"

!ENDIF 

# Begin Target

# Name "libuwin_static - Win32 Release"
# Name "libuwin_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\uconf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udebug.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\udialogx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uiphlp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ulib.cpp
# End Source File
# Begin Source File

SOURCE=..\src\uprocessman.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\uconf.h
# End Source File
# Begin Source File

SOURCE=..\inc\udebug.h
# End Source File
# Begin Source File

SOURCE=..\inc\udialog.h
# End Source File
# Begin Source File

SOURCE=..\inc\udialogx.h
# End Source File
# Begin Source File

SOURCE=..\inc\uiphlp.h
# End Source File
# Begin Source File

SOURCE=..\inc\ulib.h
# End Source File
# Begin Source File

SOURCE=..\inc\uprocessman.h
# End Source File
# End Group
# End Target
# End Project
