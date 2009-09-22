#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG
INCS = -I"../../inc"
LIBS = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_dde.o
BIN = test_dde.exe
RC  = dde.rc
RES = ${RC:.rc=.res}
#
HEADER = \
    $(INCDIR)/ulib.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_dde.o : test_dde.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

#
.PHONY : test
test :
	$(BINDIR)/$(BIN)
