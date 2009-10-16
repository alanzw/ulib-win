#
include ../make.inc

#
CXXFLAGS += -DHU_DEBUG

#
SRCDIR = ../src
INCDIR = ../inc
OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
RESDIR = ../res

DEMODIR = ../demo
DOCDIR  = ../doc

VPATH += $(INCDIR):$(INCDIR)/com 

#
include ../config.inc

OBJS_SHARED = ${OBJS:.o=_d.o}

#
all : $(OBJS) $(OBJS_SHARED)

%.o : $(SRCDIR)/%.cpp %.h
	$(CXX) -c -DBUILD_STATIC -o $@ $< -I$(INCDIR) $(CXXFLAGS)

%_d.o : $(SRCDIR)/%.cpp %.h
	$(CXX) -fPIC -c -DBUILD_DLL -o $@ $< -I$(INCDIR) $(CXXFLAGS)

#
.PHONY : clean
clean :
	$(RM) $(OBJS) $(OBJS_SHARED) uhook_d.o

.PHONY : hook
hook : uhook_d.o
#uhook_d.o : $(SRCDIR)/uhook.cpp $(INCDIR)/uhook.h


