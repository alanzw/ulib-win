#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
PYTHON_DIR  = D:/Progs/Python25
#PYTHON_DIR = C:/Python25
PYTHON_INC  = -I$(PYTHON_INC)/include
PYTHON_LIB = -L$(PYTHON_DIR)/libs -lpython25

PERL_INC = D:/Progs/Perl/lib/CORE
PERL_LIB = -LD:/Progs/Perl/lib/CORE -lperl510

TCL_DIR = D:/Progs/MinGW/ThirdParty/tcl84
TCL_INC = -I$(TCL_DIR)/include
TCL_LIB = -L$(TCL_DIR)/lib -ltcl84

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
THIRDPARTY_INC = $(THIRDPARTY_DIR)/include/lua
THIRDPARTY_LIB = -L$(THIRDPARTY_DIR)/lib -llua
#
INCS = -I"$(TOPDIR)/inc" -I$(PYTHON_DIR)/include -I$(PERL_INC) $(TCL_INC) -I$(THIRDPARTY_INC)
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin $(TCL_LIB)  $(PYTHON_LIB) $(THIRDPARTY_LIB)
CFLAGS +=
CXXFLAGS += -DBUILD_EXE


#
OBJ = \
 test_script.o \
 uscript.o     \
 utcl.o        \
 ulua.o        \
 upython.o     \
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
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

