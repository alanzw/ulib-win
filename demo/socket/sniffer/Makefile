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
OBJ = sniffer_sock.o
BIN = sniffer_sock.exe
RC  = sniffer_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
sniffer_sock.exe : sniffer_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
