#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin -lws2_32
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = network.o test_network.o
BIN = test_network.exe
RC  = network.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
test_network.exe : test_network.o network.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
 
#
.PHONY : test
test :
	@echo !!!Please run by yourself.
#	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
