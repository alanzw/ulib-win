#
include ../../make.inc

#
PYTHON_DIR  = D:/Progs/Python26
PERL_INC = D:/Progs/Perl/lib/CORE
TCL_DIR =
#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc" -I$(PYTHON_DIR)/include -I$(PERL_INC)
LIBS = -L"../../lib" -mwindows -luwin -L$(PYTHON_DIR)/libs -lpython26 -LD:/Progs/Perl/lib/CORE -lperl510 -ltcl84

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = \
 test_script.o \
 uscript.o     \
 upython.o     \
 uperl.o       \
 utcl.o

BIN = test_script.exe
RC  = script.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@


#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

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
	$(RM) ${OBJ} ${RES} ${BIN}

