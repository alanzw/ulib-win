#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc
#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
CFLAGS =
CXXFLAGS =
INC = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"


#
OBJ =
BIN =
RC  =
RES =
#
HEADER =

SUBDIRS = hello
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
