#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin -lws2_32
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = smpd.o test_smpd.o
BIN = test_smpd.exe
RC  = smpd.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
test_smpd.exe : test_smpd.o smpd.o $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@
 
#
.PHONY : test
test :
	@echo !!!Please run by yourself.
#	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
