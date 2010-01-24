#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = test_rundll.o
BIN = test_rundll.exe
RC  = rundll.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN) libuwin.dll,rdlShowMsg Say Hello
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
