#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -lcomctl32 -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = test_statusbar$(OBJEXT)
BIN = test_statusbar.exe
RC  = statusbar.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/uanimation.h \
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@

#
test_statusbar.o : test_statusbar.cpp $(HEADER)
	$(CXX) -c -DWITHOUT_DLL -o $@ $< $(CXXFLAGS) $(INC)

#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $< -o $@ -O coff -I"$(RESDIR)" -I"$(ICODIR)"

#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
