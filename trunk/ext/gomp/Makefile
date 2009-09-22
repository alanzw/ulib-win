#
include ../../make.inc

#
SUBDIRS = hello info mm chunk section reduction

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

