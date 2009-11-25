#
TOPDIR = ../../..

include $(TOPDIR)/make.inc

SUBDIRS = hello      \
          axis       \
          minimal    \
          animate    \
          3ds        \
          cube       \
          mouse      \
          bmp        \
          jpg        \
          view       \
          primitives \
          mesh2d     \
          transform  \
          clip       \
          lines      \
          pick       \
          poly       \
          bezcurve   \
          list       \
          olympic    \
          text       \
          contour2d  \
          font       \
          smooth     \
          surface    \
          quadric    \
          texture    \
          fog        \
          dib        \

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
