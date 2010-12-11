#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc


SUBDIRS = hello graph skybox tga jpeg cube camera \
          earth   \
          contour \
          gear    \
          blend   \
          tex     \

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

