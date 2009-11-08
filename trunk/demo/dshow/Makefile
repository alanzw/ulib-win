#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
SUBDIRS = dsplayer     \

#
all:
	$(MAKEALL_SUBDIRS)
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(MAKECLEAN_SUBDIRS)

