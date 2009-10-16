#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS +=
CXXFLAGS += 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin  -luuid -lole32 -loleaut32

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
OBJ = devstudio.o guids.o
BIN = devstudio.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
devstudio.exe : devstudio.o guids.o
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
	
#
%.o : %.cpp
	$(CXX) -c -DBUILD_EXE $< $(INCS) $(CXXFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
