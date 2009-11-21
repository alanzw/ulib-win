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
OBJ = test_event.o
BIN = test_event.exe
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
.PHONY : test
test:
	$(WINE) ./$(BIN)

.PHONY : clean
clean :
	$(RM) $(OBJ) $(BIN) $(RES) *.class
