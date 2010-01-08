#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"  -mwindows -luwin
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = test_cmdline.o

BIN = test_cmdline.exe
RC  = cmdline.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/udialog.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -v $@ $(BINDIR)/$@

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN) arg1 arg2

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

