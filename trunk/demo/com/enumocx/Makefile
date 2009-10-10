#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -luwin

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
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
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
