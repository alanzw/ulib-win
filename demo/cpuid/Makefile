#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = -Wno-write-strings
INC = -I"../../inc"
LIB = -L"../../lib"

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_cpuid.o
BIN = test_cpuid.exe
RC  = cpuid.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_cpuid.o : test_cpuid.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
