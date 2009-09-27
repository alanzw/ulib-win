#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS =
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
SUBDIRS = btree  \
          vector \
          stack  \
          algo   \
          sort   \
          pair   \

#
all:
	$(MAKEALL_SUBDIRS)

.PHONY : clean
clean :
	$(MAKECLEAN_SUBDIRS)
