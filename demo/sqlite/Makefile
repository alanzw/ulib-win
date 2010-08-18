#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS +=
CXXFLAGS +=
INCS += -I"$(TOPDIR)/inc"
LIBS += -L"$(TOPDIR)/lib" -L. -lsqlite3

#
OBJ = test_sqlite.o
BIN = test_sqlite.exe
RC  = sqlite.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

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
