#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"../../../inc" -I"."
LIBS = -L"../../../lib" -luwin

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
#
OBJ = enumocx.o
BIN = enumocx.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
enumocx.exe : enumocx.o
	$(CXX) -o $@ $< -luuid -lole32 -loleaut32
	cp -vf $@ $(BINDIR)/$@
	
#
%.o : %.cpp
	$(CXX) -c $< $(INCS) -DBUILD_EXE


#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
