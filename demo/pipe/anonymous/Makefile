#
include ../../../make.inc

#
INC = -I"../../../inc"
LIB = -L"../../../lib" -luwin
CFLAGS =
CXXFLAGS = 

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin
#
OBJ = test_anonymous.o
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
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

child.exe : child.cpp
	$(CXX) -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC) $(LIB)
	
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child.exe out.dat
