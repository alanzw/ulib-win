#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG
INCS = -I"../../inc"
LIBS = -L"../../lib" -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
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
test_dbgwin.o : test_dbgwin.cpp
	$(CXX) -c -o $@ $< -DBUILD_EXE $(CXXFLAGS) $(INCS)

#
test_dbg.o : test_dbg.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} test_dbgwin.exe test_dbgwin.o
#
.PHONY : test
test :
	$(BINDIR)/$(BIN)
