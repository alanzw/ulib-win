#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_mailslot.o
BIN = test_mailslot.exe
RC  = mailslot.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/umailslot.h
#
all: $(BIN) mailslot_client.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)


mailslot_client.exe : mailslot_client.o
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
	
#
.PHONY : test
test :
	$(WINE) start $(BINDIR)/$(BIN)
	$(WINE) $(BINDIR)/mailslot_client.exe
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} mailslot_client.exe
