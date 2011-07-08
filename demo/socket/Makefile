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
          httpmt   \
          httpclient\
          server   \
          ftpclient \
          chat     \
          serverui \
          wshout   \
		  broadcast\
		  keepalive\
		  select   \
		  smpd     \

#
.PHONY: all
all:
	$(MAKEALL_SUBDIRS)

.PHONY: clean
clean:
	$(MAKECLEAN_SUBDIRS)
