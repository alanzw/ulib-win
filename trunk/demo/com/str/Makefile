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
OBJ = str.o
BIN = str.exe
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
%.o : %.c
	$(CC) -c $< $(INCS)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
