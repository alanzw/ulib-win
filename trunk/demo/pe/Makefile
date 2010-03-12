#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"

#
OBJ = test_pe.o

BIN = test_pe.exe
RC  = pe.rc
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

