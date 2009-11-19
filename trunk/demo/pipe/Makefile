#
# filename : Makefile
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

#
include ../../make.inc

#
CFLAGS   =
CXXFLAGS =
INC      = -I"../../inc"
LIB      = -L"../../lib"
LDFLAGS  =
#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
BINDIR = ../../bin
LIBDIR = ../../lib
RESDIR = ../../res

DEMODIR = ../../demo

DOCDIR  = ../../doc

#
HEADER =

#
SUBDIRS = \
          anonymous \
          named     \
		  ioredirection
#
.PHONY: all
all:
	$(MAKEALL_SUBDIRS)

#
UDOC :
	cd $(DOCDIR) && $(MAKE)

install :

.PHONY: clean
clean:
	$(MAKECLEAN_SUBDIRS)

