#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = -DBUILD_EXE
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin

#
OBJ = test_seh.o
BIN = test_seh.exe
RC  = seh.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)S

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
