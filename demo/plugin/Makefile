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
SUBDIRS = plugin1 \
          plugin2

#
all: $(BIN) plugins

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_plugin.o : test_plugin.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)


plugins :
	${MAKEALL_SUBDIRS}

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
	$(MAKECLEAN_SUBDIRS)

