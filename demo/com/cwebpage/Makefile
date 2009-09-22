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
OBJ = cwebpage.o
BIN = cwebpage.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
cwebpage.exe : cwebpage.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32 -mwindows

#
%.o : %.c
	$(CC) -c $< $(INCS)

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
