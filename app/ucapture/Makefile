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
OBJ = test_capture.o \
      ucapture.o


BIN = test_capture.exe
RC  = capture.rc
RES = ${RC:.rc=.res}
#
HEADER = ucapture.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

