#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin -lmsimg32
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_static.o

BIN = test_static.exe
RC  = static.rc
RES = ${RC:.rc=.res}
#
HEADERS= $(INCDIR)/ucontrol.h \
		 $(INCDIR)/ustatic.h \
		 $(INCDIR)/udialog.h \
		 ulcd.h \
		 uled.h \
		 uhistogram.h \
		 uinfobar.h   \
		 umeterctrl.h \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

