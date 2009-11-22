#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"  -mwindows -luwin
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE -Wno-write-strings
LDFLAGS += $(LIBS)

#
OBJ = test_criticalsection.o

BIN = test_criticalsection.exe
RC  = criticalsection.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucritical.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

