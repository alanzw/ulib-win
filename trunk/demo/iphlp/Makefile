#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIB = -L"../../lib" -liphlpapi

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico

BINDIR = ../../bin
#
OBJ = test_iphlp.o

BIN = test_iphlp.exe
RC  = iphlp.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -v $(BIN) $(BINDIR)/$(BIN)

#
test_iphlp.o : test_iphlp.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
#
.PHONY : test
test :
	$(WINE) ${BINDIR}/${BIN}
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

