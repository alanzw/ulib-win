#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../../inc"
LIB = -L"../../../lib" -luwin

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin
#
OBJ = test_ioredirect.o \
      $(OBJDIR)/upipe.o
BIN = test_ioredirect.exe
RC  = ioredirect.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/upipe.h
#
all: $(BIN) child_ioredirect.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_ioredirect.o : test_ioredirect.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

$(OBJDIR)/upipe.o : $(SRCDIR)/upipe.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

child_ioredirect.exe : child_ioredirect.cpp ioredirect.res
	$(CXX) -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC) $(LIB)
	cp -vf $@ $(BINDIR)/$@
	
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN) test.input
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child_ioredirect.exe child_ioredirect.o
