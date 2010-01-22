#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc


#
HEADER =

SUBDIRS = wowaccess
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
