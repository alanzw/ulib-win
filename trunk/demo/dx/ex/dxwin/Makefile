#
TOPDIR = ../../../..
#
include $(TOPDIR)/make.inc


#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
DXSDK_DIR = D:/Progs/DirectXSDK

#
CFLAGS =
CXXFLAGS = -fcheck-new -DDECLSPEC_DEPRECATED=
INC = -I"$(TOPDIR)/inc"
LIB = -L"$(TOPDIR)/lib" -mwindows -luwin -ld3d9
#
OBJ = test_dxwin.o

BIN = test_dxwin.exe
RC  = dxwin.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -v $@ $(BINDIR)/$@

#
test_dxwin.o : test_dxwin.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

