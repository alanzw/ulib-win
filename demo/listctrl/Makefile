#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin  -lcomctl32
CFLAGS +=
CXXFLAGS += -DBUILD_EXE
#
OBJ = test_listctrl.o

BIN = test_listctrl.exe
RC  = listctrl.rc
RES = ${RC:.rc=.res}
#
HEADERS = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/ulistview.h \
		 $(INCDIR)/udialogx.h \
		 custom_listctrl.h \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@
    
#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

