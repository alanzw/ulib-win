#
TOPDIR = ../../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin  -ld3d9
CFLAGS +=
CXXFLAGS += -DBUILD_EXE -fcheck-new -DDECLSPEC_DEPRECATED=
#
DXSDK_DIR = D:/Progs/DirectXSDK

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
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@


.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

