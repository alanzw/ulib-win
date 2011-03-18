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
OBJ = test_genetic.o \


BIN = test_genetic.exe
RC  = genetic.rc
RES = ${RC:.rc=.res}
#
HEADERS = 
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.o *.exe

