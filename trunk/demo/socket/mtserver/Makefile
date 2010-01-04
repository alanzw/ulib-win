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
OBJ = mtserver_sock.o mtclient_sock.o
BIN = mtserver_sock.exe mtclient_sock.exe
RC  = mtserver_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
mtserver_sock.exe : mtserver_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
#
mtclient_sock.exe : mtclient_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
 
#
.PHONY : test
test :
	echo Please run by yourself.
#	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
