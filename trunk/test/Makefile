#
# filename : Makefile
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

#
include ../make.inc

#
CFLAGS   =
CXXFLAGS =
INC      = -I"../inc"
LIB      = -L"../lib"
LDFLAGS  =
#
SRCDIR = ../src
INCDIR = ../inc
OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
RESDIR = ../res

DEMODIR = ../demo

DOCDIR  = ../doc

#
HEADER =

#
SUBDIRS =        \
      adt        \
      algo       \
      vec        \
      funcptr    \
      factory    \
      nonthrow   \
      smartptr   \
      thintempl  \
      handlebody \
      exprtempl  \
      delegate   \
      observer   \
      builder    \
      objectpool
#
.PHONY: all
all:
	$(MAKEALL_SUBDIRS)
#	for X in $(SUBDIRS);\
#	do \
#	  (cd $$X && $(MAKE)); \
#	done

#
UDOC :
	cd $(DOCDIR) && $(MAKE)

install :

.PHONY: clean
clean:
	$(MAKECLEAN_SUBDIRS)
#	for X in $(SUBDIRS);\
#	do \
#		(cd $$X && $(MAKE) clean); \
#	done
