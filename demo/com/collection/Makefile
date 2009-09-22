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
OBJ = IExample3.o PortNames.o test.o reg.o unreg.o
BIN = IExample3.dll PortNames.o test.exe reg.exe unreg.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
IExample3.dll : IExample3.o PortNames.o
	$(CC) -shared -o $@ $^ -Wl,--kill-at -luuid -loleaut32

test.exe : test.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32

reg.exe : reg.o
	$(CC) -o $@ $< -lcomdlg32 -lole32 -loleaut32 -mwindows
 
unreg.exe : unreg.o
	$(CC) -o $@ $< -lcomdlg32 -lole32 -loleaut32 -mwindows
    
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
