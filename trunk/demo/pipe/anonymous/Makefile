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
OBJ = test_anonymous.o \
      $(OBJDIR)/upipe.o
BIN = test_anonymous.exe
RC  = anonymous.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/upipe.h
#
all: $(BIN) child.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_anonymous.o : test_anonymous.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

$(OBJDIR)/upipe.o : $(SRCDIR)/upipe.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

child.exe : child.cpp
	$(CXX) -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC) $(LIB)
	
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child.exe out.dat
