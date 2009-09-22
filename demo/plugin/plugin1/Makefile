#
include ../../../make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG
INCS = -I"../../../inc"
LIBS = -L"../../../lib" -luwin

#
SRCDIR = ../../../src
INCDIR = ../../../inc
OBJDIR = ../../../obj
RESDIR = ../../../res
ICODIR = ../../../ico
BINDIR = ../../../bin
#
OBJ = plugin1.o \
      ulib.o
BIN = plugin1.dll
RC  =
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uplugin.h \
         $(INCDIR)/ulib.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -shared -o $@ $^ -Wl,--out-implib,libplugin1.dll.a $(LIBS)
	cp -v $@ $(BINDIR)/$@

ulib.o : $(SRCDIR)/ulib.cpp $(HEADER)
	$(CXX) -fPIC -c -o $@ $< -DBUILD_DLL $(CXXFLAGS) $(INCS)

#
plugin1.o : plugin1.cpp $(HEADER)
	$(CXX) -fPIC -c -o $@ -DBUILD_DLL $< $(CXXFLAGS) $(INCS)

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    

#
.PHONY : test
test :
	cd $(BINDIR) && ./$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
	$(RM) *.a
