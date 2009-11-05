#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS +=
CXXFLAGS += 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -mwindows -luuid -lole32 -loleaut32

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
OBJ = test_bho.o eventsink.o objectwithsite.o classfactory.o
BIN = test_bho.dll
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
test_bho.dll : $(OBJ)
	$(CXX) -shared -o $@ $^ $(LIBS) -Wl,--kill-at
	cp -vf $@ $(BINDIR)/$@
	
#
%.o : %.cpp
	$(CXX) -c -DBUILD_DLL $< $(INCS) $(CXXFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
