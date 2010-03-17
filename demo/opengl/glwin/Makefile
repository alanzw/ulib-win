#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../../inc"
LIBS = -L"../../../lib" -mwindows -luwin

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin
#
OBJ =
BIN =
RC  =
RES =
#
HEADER =

SUBDIRS = hello graph skybox
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

