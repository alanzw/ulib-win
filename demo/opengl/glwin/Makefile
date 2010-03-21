#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc


SUBDIRS = hello graph skybox tga jpeg
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

