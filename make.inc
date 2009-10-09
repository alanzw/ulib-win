#
# filename : make.inc
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

####################################
# The building environment
####################################

#PLATFORM = win32
PLATFORM = linux32

####################################
# Under Linux, MinGW programs are
# named with the prifix:
# (1) i486-mingw32msvc- (ubuntu)
# (2) i586-mingw32msvc- (ubuntu)
# (3) i386-mingw32- (fedora)
# * If building under Linux,
# * please uncomment this line.
####################################

#PRE_MINGW = i586-mingw32msvc-
#PRE_MINGW = i386-mingw32-

ifeq ($(PLATFORM), linux32)
    PRE_MINGW = i586-mingw32msvc-
#    PRE_MINGW = i386-mingw32-
else
    PRE_MINGW =
endif

#
INSTALL =

#
uSE_UNICODE = no

#
USE_DEBUG = yes

#
USE_STATIC = no

#
USE7Z = yes

#
MAKE = make
RM   = -rm -vf
AR   = $(PRE_MINGW)ar rcs
STRIP= $(PRE_MINGW)strip
#
CC = $(PRE_MINGW)gcc
CXX = $(PRE_MINGW)g++
WINDRES = $(PRE_MINGW)windres
WINDMC  = $(PRE_MINGW)windmc
#
SRCEXT = .cpp
OBJEXT = .o
BINEXT = .exe

#
ifeq ($(USE_DEBUG), yes)
    CFLAGS   = -g
    CXXFLAGS = -g
else
    CFLAGS   =
    CXXFLAGS =
endif

INCS     =
LIBS     =

#
STATIC_LD = -static

ifeq ($(USE_STATIC), yes)
    LDFLAGS  = $(STATIC_LD)
else
    LDFLAGS  =
endif
    
RCFLAGS  = -DMINGW_RC  -O coff -I"$(RESDIR)" -I"$(ICODIR)"

#
ifeq ($(PLATFORM), win32)
    SHELL = cmd.exe
    WINE =
    CLEANUP = wbin\find . \( -name Thumbs.db -o -name *.aps \) -type f -print0 -delete
    P7ZIP = "./ulib-win/wbin/7z.exe"
    MAKEALL_SUBDIRS = for /d %%X in ($(SUBDIRS)) do $(MAKE) -C %%X
    MAKECLEAN_SUBDIRS = for /d %%X in ($(SUBDIRS)) do $(MAKE) clean -C %%X
else
    SHELL = /bin/bash
    WINE = LC_CTYPE="zh_CN.UTF-8" /opt/build/wine/bin/wine
    CLEANUP = find . \( -name Thumbs.db -o -name *.aps \) -type f -print0 | xargs -0 /bin/rm -vf
    P7ZIP = 7z
    MAKEALL_SUBDIRS = for X in $(SUBDIRS);do (cd $$X && $(MAKE) all); done
    MAKECLEAN_SUBDIRS = for X in $(SUBDIRS);do (cd $$X && $(MAKE) clean); done
endif

#
%.res : %.rc
	$(WINDRES) -i $< -o $@ $(RCFLAGS)
