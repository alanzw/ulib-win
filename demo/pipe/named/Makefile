#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE
#
OBJ = test_named.o
BIN = test_named.exe
RC  = named.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/upipe.h
#
all: $(BIN) child.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
child.exe : child.cpp
	$(CXX) -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC) $(LIB)
	
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child.exe out.dat
