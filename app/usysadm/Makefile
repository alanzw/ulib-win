#
include ../../make.inc

#
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_sysadm.o \


BIN = test_sysadm.exe
RC  = sysadm.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h \
          ./usysadm.h

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_sysadm.o : test_sysadm.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

.PHONY : test
test :
	$(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

