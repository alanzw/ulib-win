#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin -lws2_32 -lcrypt32 -lsecur32 
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = ssl.o tel.o
BIN = tel.exe
RC  = ssl.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
tel.exe : $(OBJ) $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)
 
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
