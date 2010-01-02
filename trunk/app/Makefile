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
SUBDIRS = u360safe uchart ukav uqq uscript utaskman \
          uwget uviewer ufluent usysadm ucontour ublogger \
          usdp uctrls unetchat ubtnst udeskutil ugrid ulife \
          uterminal ucontact uhys uctrlbar u3dcreator usui \
          unumerical uglsurf ucalc usort umytree uguievent \
          uexplorer

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

