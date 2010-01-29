#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS += $(LIBS)

#
OBJ = test_cpl.o
BIN = test_cpl.exe
RC  = cpl.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) hy.cpl

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
hy.cpl : hy.cpp $(RES)
	$(CXX) -fPIC -c -o hy.o $^ -DBUILD_DLL $(INCS) 
	g++ -shared -o $@ hy.o $(RES) -Wl,--kill-at
	cp -vf $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN) hy.cpl
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.cpl *.o
