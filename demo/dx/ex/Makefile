#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../../inc"
LIB = -L"../../../lib" -mwindows -ld3d9

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin
#
SUBDIRS = dxwin     \

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

