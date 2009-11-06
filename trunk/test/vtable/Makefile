#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS += $(INCS)
CXXFLAGS += $(INCS) -fdump-class-hierarchy
LDFLAGS += $(LIBS)

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
OBJ = test_vtable.o
BIN = test_vtable.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LDFLAGS) $(RES)

#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

#
.PHONY : test
test:
	$(WINE) ./$(BIN)

.PHONY : clean
clean :
	$(RM) $(OBJ) $(BIN) $(RES) *.class
