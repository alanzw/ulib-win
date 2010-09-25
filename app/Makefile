#
# filename : Makefile
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

#
include ../make.inc

#
SUBDIRS = u360safe uchart ukav uqq uscript utaskman \
          uwget uviewer ufluent usysadm ucontour ublogger \
          usdp uctrls unetchat ubtnst udeskutil ugrid ulife \
          uterminal ucontact uhys uctrlbar u3dcreator usui \
          unumerical uglsurf ucalc usort umytree uguievent \
          uexplorer uincome urefman ucapture uclock uqq2009 \
          utabbar utetrix ugo uads ufilesplit ueditpad uwdemo \
          ubooks uairfoil

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

