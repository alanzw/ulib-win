#
include ../../make.inc

#
CFLAGS =
CXXFLAGS =
INCS = -I"../../inc"
LIBS = -L"../../lib"

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = test_functor.o
BIN = test_functor.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/adt/ualgo.h \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS) $(RES)

#
test_funcptr.o : test_funcptr.cpp $(HEADER)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCS)

$(RES) : $(RC) resource.h
	$(WINDRES) -i $< -o $@ -O coff -I"$(RESDIR)" -I"$(ICODIR)"
#
.PHONY : test
test:
	$(WINE) ./$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
