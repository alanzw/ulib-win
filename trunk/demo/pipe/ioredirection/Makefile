#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_ioredirect.o \
      $(OBJDIR)/upipe.o
BIN = test_ioredirect.exe
RC  = ioredirect.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/upipe.h
#
all: $(BIN) child_ioredirect.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

child_ioredirect.exe : child_ioredirect.cpp ioredirect.res
	$(CXX) -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC) $(LIB)
	cp -vf $@ $(BINDIR)/$@
	
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN) test.input
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child_ioredirect.exe child_ioredirect.o
