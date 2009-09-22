#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"../../../inc" -I"."
LIBS = -L"../../../lib" -luwin -lws2_32

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin

#
OBJ = echoserver_sock.o echoclient_sock.o
BIN = echoserver_sock.exe echoclient_sock.exe
RC  = echo_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
echoserver_sock.exe : echoserver_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
#
echoclient_sock.exe : echoclient_sock.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
#
%.o : %.cpp
	$(CXX) -c -o $@ $< -DBUILD_EXE $(INCS)


#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
 
#
.PHONY : test
test :
	echo Please run by yourself.
#	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
