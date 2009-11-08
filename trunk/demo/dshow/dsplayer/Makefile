#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS +=
CXXFLAGS += 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -mwindows -luuid -lole32 -loleaut32 -lcomctl32 -lcomdlg32 -lstrmiids

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
OBJ = dsplayer.o playerclass.o
BIN = dsplayer.exe
RC  = dsplayer.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
$(BIN) : $(OBJ) $(RES)
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
