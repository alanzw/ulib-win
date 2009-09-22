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
BINDIR = ../../../bin
#
OBJ = rawping.o ip_checksum.o rawping_driver.o
BIN = rawping.exe
RC  = rawping.rc
RES = ${RC:.rc=.res}
#
HEADER = rawping.h ip_checksum.h
#
all: $(BIN) 

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $@ $^ -lws2_32
	cp -vf $@ $(BINDIR)/$@ 
#
%.o : %.c
	$(CXX) -c $< $(INCS)


#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
