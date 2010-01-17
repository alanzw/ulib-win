#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -luwin -luuid -lole32 -loleaut32 -mwindows 

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
OBJS = test_ipicture.o
BIN = test_ipicture.exe
RC  = ipicture.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
$(BIN) : $(OBJS) $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
	
#
%.o : %.cpp
	$(CXX) -c $< $(INCS) -DBUILD_EXE $(CXXFLAGS)

#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJS} ${RES} ${BIN}
