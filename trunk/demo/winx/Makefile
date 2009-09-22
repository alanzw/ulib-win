#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = \
 test_winx.o

BIN = test_winx.exe
RC  = winx.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uwindowx.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -v $@ $(BINDIR)/$@

#
test_winx.o : test_winx.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)


#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
.PHONY : test
test :
	$(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

