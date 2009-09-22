#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
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
OBJ = test_ipaddr$(OBJEXT)
BIN = test_ipaddr.exe
RC  = ipaddr.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/uipaddress.h \
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@

#
test_ipaddr.o : test_ipaddr.cpp $(HEADER)
	$(CXX) -c -DBUILD_EXE -o $@ $< $(CXXFLAGS) $(INC)

$(RES) : $(RC) resource.h
	$(WINDRES) -i $< -o $@ -DMINGW_RC -O coff -I"$(RESDIR)" -I"$(ICODIR)"
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
