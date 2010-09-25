#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin -lws2_32  -mwindows
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = chat_sock.o
BIN = chat_sock.exe
RC  = chat_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
chat_sock.exe : chat_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
 
#
.PHONY : test
test :
#	echo Please run by yourself.
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
