#
include ../../make.inc

#
CFLAGS +=
CXXFLAGS += 
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_static.o

BIN = test_static.exe
RC  = static.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/ustatic.h \
		 $(INCDIR)/udialog.h \
		 ulcd.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -v $@ $(BINDIR)/$@

#
test_static.o : test_static.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

