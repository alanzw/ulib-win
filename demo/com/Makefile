#
# filename : Makefile
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
SUBDIRS = \
          hello     \
          cwebpage  \
          str       \
          var       \
          script    \
          collection\
          enumocx   \
          devstudio \
          ipicture  \
          istream   \
          memstream \
          rpc       \
          iimgctx   \
          webbrowser\
          sstorage  \
          bho       \
          ado       \
          

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
