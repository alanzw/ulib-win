#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ =

BIN =
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
SUBDIRS = ex      \
          glctrl  \
          glwin   \

#
all:
	$(MAKEALL_SUBDIRS)
#
.PHONY : clean
clean :
	$(MAKECLEAN_SUBDIRS)
