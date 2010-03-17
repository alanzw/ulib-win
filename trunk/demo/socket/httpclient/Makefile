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
OBJ = httpclient_sock.o httpclient.o
BIN = httpclient_sock.exe
RC  = httpclient_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
httpclient_sock.exe : httpclient_sock.o httpclient.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
	
#
%.o : %.cpp
	$(CXX) -c -o $@ $< -DBUILD_EXE $(INCS)
 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
