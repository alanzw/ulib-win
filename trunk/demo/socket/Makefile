#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
SUBDIRS =          \
          simple   \
          echo     \
          rawping  \
          ping     \
          portscan \
          webserv  \
          iocp     \
          ftpserver\
          sniffer  \
          ipv6     \
          mtserver \
          interface\
          portmap  \

#
.PHONY: all
all:
	$(MAKEALL_SUBDIRS)

.PHONY: clean
clean:
	$(MAKECLEAN_SUBDIRS)
