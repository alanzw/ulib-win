#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
#PYTHON_DIR  = D:/Progs/Python26
PYTHON_DIR = C:/Python25
PYTHON_LIB = -L$(PYTHON_DIR)/libs -lpython25
PERL_INC = D:/Progs/Perl/lib/CORE
PERL_LIB = -LD:/Progs/Perl/lib/CORE -lperl510
TCL_DIR =
THIRDPARTY_DIR = $(TOPDIR)/thirdparty
THIRDPARTY_INC = $(THIRDPARTY_DIR)/include/lua
THIRDPARTY_LIB = $(THIRDPARTY_DIR)/lib/liblua.a
#
CFLAGS =
CXXFLAGS =
INC = -I"../../inc" -I$(PYTHON_DIR)/include -I$(PERL_INC) -I$(THIRDPARTY_INC)
LIBS = -L"../../lib" -mwindows -luwin -ltcl84  $(THIRDPARTY_LIB)

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
 utcl.o        \
 ulua.o        \
# upython.o     \
# uperl.o       \

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

