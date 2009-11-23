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
OBJ = test_mc.o
BIN = test_mc.exe
RC  = test_mc.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/umsg.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_mc.o : test_mc.cpp $(HEADER) mc.h
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

    
mc.h mc.rc : mc.mc
	$(WINDMC) -u -U mc.mc

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} Message* mc.h mc.rc
