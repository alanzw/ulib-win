#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_ipaddr$(OBJEXT)
BIN = test_ipaddr.exe
RC  = ipaddr.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/uipaddress.h \
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS) $(RES)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
