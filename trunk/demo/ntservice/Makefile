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
OBJ = test_ntservice.o
BIN = test_ntservice.exe
RC  = ntservice.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/untservice.h
#
all: $(BIN) hyservice.exe xyntservice.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

hyservice.exe : hyservice.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)

xyntservice.exe : xyntservice.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.o *.exe
