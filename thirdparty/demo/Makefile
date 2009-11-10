#
TOPDIR = ../..

include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
OBJ =

BIN =
RC  =
RES = ${RC:.rc=.res}
#
HEADER =

SUBDIRS = glut      \

#
all:
	$(MAKEALL_SUBDIRS)

#
.PHONY : clean
clean :
	$(MAKECLEAN_SUBDIRS)
