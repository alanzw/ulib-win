#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE -DHU_DEBUG
#
OBJ = test_dbg.o
BIN = test_dbg.exe
RC  = debug.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udebug.h \
         $(INCDIR)/ulib.h
#
all: $(BIN) test_dbgwin.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_dbgwin.exe : test_dbgwin.o $(RES)
	$(CXX) -o $@ $^ $(LIBS) -mwindows
	cp -v $@ $(BINDIR)/$@

    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} test_dbgwin.exe test_dbgwin.o
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
