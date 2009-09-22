#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG
INCS = -I"../../inc"
LIBS = -L"../../lib" -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_plugin.o
BIN = test_plugin.exe
RC  = plugin.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uplugin.h \
         $(INCDIR)/ulib.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_plugin.o : test_plugin.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)

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
