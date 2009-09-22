#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"../../inc"
LIBS = -L"../../lib" -luwin

#
SRCDIR = ../../src
INCSDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_fileman.o
BIN = test_fileman.exe
RC  = fileman.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCSDIR)/ufileman.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_fileman.o : test_fileman.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
