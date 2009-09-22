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
OBJ = portscan_sock.o
BIN = portscan_sock.exe
RC  = portscan_sock.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
	
#
%.o : %.cpp
	$(CXX) -c -o $@ $< -DBUILD_EXE $(INCS)


#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
