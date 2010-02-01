#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
SUBDIRS = \
          db          \
          db_dll      \
          db_vtbl     \
          db_vtbl_dll \
          db_almostcom\
          db_com      \
          db_commul   \
          db_comrem   \

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
