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
BINDIR = ../../../bin
#
OBJ = hello.o reg.o unreg.o test.o test_cpp.o
BIN = hello.dll reg.exe unreg.exe test.exe test_cpp.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
hello.dll : hello.o
	$(CC) -shared -o $@ $< -Wl,--kill-at -luuid -loleaut32
	
reg.exe : reg.o
	$(CC) -o $@ $< -lcomdlg32 -mwindows

unreg.exe : unreg.o
	$(CC) -o $@ $< -lcomdlg32 -mwindows
	
test.exe : test.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32
	
test_cpp.exe : test_cpp.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32
#
%.o : %.c
	$(CC) -c $<

%.o : %.cpp
	$(CXX) -c $<
    
test_noreg.exe : test_noreg.cpp
	$(CXX) -o  $@ $< $(INC) $(LIB) -DBUILD_EXE -luwin -luuid -lole32 -loleaut32
	cp -vf $@ $(BINDIR)/$@
#
.PHONY : test
test:
	$(WINE) $(BINDIR)/test_noreg.exe
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} test_noreg.exe
