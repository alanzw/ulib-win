#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = \
 test_qq.o \

BIN = test_qq.exe
RC  = qq.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_qq.o : test_qq.cpp $(HEADER)
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

