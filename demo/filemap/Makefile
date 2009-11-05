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
OBJ = test_filemap.o
BIN = test_filemap.exe
RC  = filemap.rc
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
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)
    
#
misc : first.o second.o fnfh.o $(RES)
	$(CXX) -o first.exe first.o $(RES)
	$(CXX) -o second.exe second.o $(RES)
	$(CXX) -o fnfh.exe fnfh.o $(RES) -lpsapi   
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.exe *.o *.res
