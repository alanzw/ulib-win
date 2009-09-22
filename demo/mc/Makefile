#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIBS = -L"../../lib" -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_mc.o
BIN = test_mc.exe
RC  = test_mc.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/umsg.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_mc.o : test_mc.cpp $(HEADER) mc.h
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
$(RES) : $(RC) resource.h mc.rc
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

    
mc.h mc.rc : mc.mc
	$(WINDMC) -u -U mc.mc

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} Message* mc.h mc.rc
