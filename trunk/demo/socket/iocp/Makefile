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
OBJ = iocpsrv_sock.o iocpcli_sock.o
BIN = iocpsrv_sock.exe iocpcli_sock.exe
RC  = iocp_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
iocpsrv_sock.exe : iocpsrv_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
#
iocpcli_sock.exe : iocpcli_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@ 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
