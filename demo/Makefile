#
# filename : Makefile
# author   : huys
# updated  : 2008.11.21.
# version  : 0.1
#

#
include ../make.inc

#
CFLAGS   =
CXXFLAGS =
INC      = -I"../inc"
LIB      = -L"../lib"
LDFLAGS  =
#
SRCDIR = ../src
INCDIR = ../inc
OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
RESDIR = ../res

DEMODIR = ../demo

DOCDIR  = ../doc

#
HEADER =

#
SUBDIRS = \
          animation   \
          bitmap      \
          button      \
          clipboard   \
          combobox    \
          comboboxex  \
          conf        \
          console     \
          cursor      \
          datetimepick\
          dde         \
          ddk         \
          debug       \
          dialog      \
          dll         \
          dx          \
          edit        \
          error       \
          event       \
          exception   \
          fileman     \
          filemap     \
          font        \
          gdi         \
          gdiplus     \
          headerctrl  \
          hook        \
          ghook       \
          icon        \
          imagelist   \
          iphlp       \
          link        \
          listbox     \
          listctrl    \
          mc          \
          menu        \
          mouse       \
          msg         \
          opengl      \
          pipe        \
          plugin      \
          process     \
          progressbar \
          rebar       \
          regkey      \
          rt          \
          scrollbar   \
          shell       \
          skin        \
          static      \
          statusbar   \
          string      \
          superclass  \
          sysinfo     \
          tabctrl     \
          thread      \
          thunk       \
          toolbar     \
          tooltip     \
          trayicon    \
          treectrl    \
          updown      \
          window      \
          winx        \
          commondlg   \
          resource    \
          com         \
          process     \
          processman  \
          keyboard    \
          pipe        \
          ntservice   \
          draglistbox \
          richedit    \
          filedb      \
          rundll      \
          ipaddr      \
          rebar       \
          sqlite      \
          font        \
          panel       \
          mutex       \
          socket      \
          splash      \
          time        \
          timer       \
          file        \
          ini         \
          winapp      \
          floatwnd    \
          cmdline     \
          sdi         \
          mdi         \
          mci         \
          shell       \
          fullscreen  \
          cpuid       \
          asm         \
          nls         \
          xml         \
          shell       \
          layout      \
          rgn         \
          customctrl  \
          messageonly \
          sigslot     \
          trackbar    \
          toolhelp    \
          dc          \
          dock        \
          splitter    \
          apihook     \
          thunk32     \
          semaphore   \
          criticalsection \
          visualstyle \
          sync        \


#
all:
	$(MAKEALL_SUBDIRS)
#
UDOC :
	cd $(DOCDIR) && $(MAKE)

install :

.PHONY: clean
clean:
	$(MAKECLEAN_SUBDIRS)

