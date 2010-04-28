#
TOPDIR = ..
#
include $(TOPDIR)/make.inc

## !! Make sure shlwapi is the first, or StrStrIA's location will be confusing.
LIBS = -mwindows -lshlwapi -lcomctl32 -lnetapi32 -lpsapi  -liphlpapi -lopengl32 -lglu32 -lws2_32 -lwinspool -lwinmm -luuid -lole32 -loleaut32 -luxtheme -lodbc32
#LIBS = -mwindows -lcomctl32

#
include $(TOPDIR)/config.inc

OBJECTS = $(OBJS)

OBJECTS_SHARED = ${OBJECTS:.o=_d.o}

#
OBJECTS_E = $(addprefix $(OBJDIR)/, ${OBJECTS})
#OBJECTS_SHARED_E = $(addprefix, $(OBJDIR)/, ${OBJECTS_SHARED})
OBJECTS_SHARED_E = ${OBJECTS_E:.o=_d.o}

RC  = $(RESDIR)/version.rc
RES = ${RC:.rc=.res}

#
all : STATIC_LIB SHARED_LIB

STATIC_LIB : libuwin.a
libuwin.a : ${OBJECTS_E}
	$(AR) $@ $^

SHARED_LIB : libuwin.dll libuwin.dll.a
libuwin.dll : ${OBJECTS_SHARED_E}
	$(CXX) -shared -o $@ $^ -Wl,--output-def,libuwin.def,--out-implib,libuwin.dll.a $(LIBS) $(RES)
	cp -v $@ $(BINDIR)/$@

$(OBJECTS_E) $(OBJECTS_SHARED_E) :
	$(MAKE) -C $(OBJDIR)

.PHONY : clean
clean :
	$(RM) libuwin.a libuwin.dll.a libuwin.dll libuwin.def uhook.dll libuhook.dll.a libuhook.def

.PHONY : hook
hook : uhook.dll
uhook.dll : $(OBJDIR)/uhook_d.o
	$(CXX) -shared -o $@ $^ -Wl,--output-def,libuhook.def,--out-implib,libuhook.dll.a -luwin -L$(LIBDIR)
	cp -v $@ $(BINDIR)/$@

$(OBJDIR)/uhook_d.o :
	$(MAKE) -C $(OBJDIR) hook

