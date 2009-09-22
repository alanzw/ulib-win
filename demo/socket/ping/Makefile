#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"../../../inc" -I"."
LIBS = -L"../../../lib" -luwin

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin

#
OBJ = ping_sock.o
BIN = ping_sock.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
ping_sock.exe : ping_sock.o
	$(CXX) -o $@ $< $(LIBS)
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
