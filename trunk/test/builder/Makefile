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
OBJ = test_builder.o
BIN = test_builder.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS) $(RES)

#
test_builder.o : test_builder.cpp $(HEADER)
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
