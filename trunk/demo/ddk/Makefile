#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

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
