#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"  -mwindows -luwin
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE -Wno-write-strings
LDFLAGS += $(LIBS)
#
OBJ = test_cpuid.o
BIN = test_cpuid.exe
RC  = cpuid.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) mycpuid.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

mycpuid.exe : mycpuid.o $(RES)
	$(CXX) -o $@ $^ $(LIB)
	cp -vf $@ $(BINDIR)/$@
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
