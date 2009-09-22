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

SUBDIRS = hello
#
all:
	for /d %%X in ($(SUBDIRS)) do $(MAKE) -C %%X
#	for X in $(SUBDIRS);\
#	do \
#	  (cd $$X && $(MAKE) all); \
#	done

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

 #
.PHONY : clean
clean :
	for /d %%X in ($(SUBDIRS)) do $(MAKE) clean -C %%X
#	for X in $(SUBDIRS);\
#	do \
#	  (cd $$X && $(MAKE) clean); \
#	done

