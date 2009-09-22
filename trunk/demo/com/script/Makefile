#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../../inc"
LIB = -L"../../../lib"

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
#
OBJ = IExample2.o reg.o unreg.o test.o
BIN = IExample2.dll reg.exe unreg.exe test.exe 
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
IExample2.dll : IExample2.o
	$(CC) -shared -o $@ $< -Wl,--kill-at -luuid -loleaut32
	
reg.exe : reg.o
	$(CC) -o $@ $< -lcomdlg32 -lole32 -loleaut32 -mwindows

unreg.exe : unreg.o
	$(CC) -o $@ $< -lcomdlg32 -lole32 -loleaut32 -mwindows
	
test.exe : test.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32
	
#
%.o : %.c
	$(CC) -c $<

%.o : %.cpp
	$(CXX) -c $<

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
