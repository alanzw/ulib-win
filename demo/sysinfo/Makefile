#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS += -DHU_DEBUG -DBUILD_EXE
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin -lpsapi
#
OBJ = test_sys.o
BIN = test_sys.exe
RC  = sys.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/usys.h \
         $(INCDIR)/ulib.h
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
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
