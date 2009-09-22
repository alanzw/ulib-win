#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
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
OBJ =

BIN =
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all:
	(cd ex && $(MAKE) all)
	(cd glctrl && $(MAKE) all)
	(cd glwin && $(MAKE) all) 
#
#$(BIN) : $(OBJ) $(RES)
#	$(CXX) -o $(BIN) $^ $(LIBS)
#	cp -v $@ $(BINDIR)/$@

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	(cd ex && $(MAKE) clean)
	(cd glctrl && $(MAKE) clean)
	(cd glwin && $(MAKE) clean) 
