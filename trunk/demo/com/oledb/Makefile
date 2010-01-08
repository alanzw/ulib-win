#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS =
INCS = -I"../../../inc" -I"."
LIBS = -L"../../../lib"

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
#
OBJ = oledb.o
BIN = oledb.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
str.exe : str.o
	$(CXX) -o $@ $< -loleaut32

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
