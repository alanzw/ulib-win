#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

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
.PHONY : test
test :
	$(WINE) ${BINDIR}/${BIN}
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

