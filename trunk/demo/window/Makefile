#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -lmsimg32

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = \
 test_window.o \
 $(OBJDIR)/uwindow.o

BIN = test_window.exe
RC  = window.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uwindow.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $@ $(BINDIR)/$@

#
test_window.o : test_window.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

$(OBJDIR)/uwindow.o : $(SRCDIR)/uwindow.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

.PHONY: test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

