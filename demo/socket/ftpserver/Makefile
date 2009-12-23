#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = ftpserver_sock.o
BIN = ftpserver_sock.exe
RC  = ftpserver_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
ftpserver_sock.exe : ftpserver_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
