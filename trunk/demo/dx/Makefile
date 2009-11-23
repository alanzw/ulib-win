#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
SUBDIRS = ex     \

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

