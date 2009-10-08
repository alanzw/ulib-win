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

SUBDIRS = hello      \
          axis       \
          minimal    \
          animate    \
          3ds        \
          cube       \
          mouse      \
          bmp        \
          jpg        \
          view       \
          primitives \
          mesh2d     \
          transform  \
          clip       \
          lines      \
          pick       \
          poly       \
          bezcurve   \
          list       \
          olympic    \
          text       \
          contour2d  \
          font       \
          smooth     \
          surface    \
          quadric    \
          texture    \
          fog        \

#
all:
	$(MAKEALL_SUBDIRS)
#	for /d %%X in ($(SUBDIRS)) do $(MAKE) -C %%X
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
	$(MAKECLEAN_SUBDIRS)
#	for /d %%X in ($(SUBDIRS)) do $(MAKE) clean -C %%X
#	for X in $(SUBDIRS);\
#	do \
#	  (cd $$X && $(MAKE) clean); \
#	done

