#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

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
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $(BIN) $(BINDIR)/$(BIN)

#
.PHONY : test
test :
	$(WINE) ${BINDIR}/${BIN}
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

