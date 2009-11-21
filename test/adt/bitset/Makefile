#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=
CXXFLAGS += $(INCS)
LDFLAGS += $(LIBS) -Wl,-Map,out.map -Wl,--cref

#
OBJ = test_bitset.o
BIN = test_bitset.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/adt/ubinary_tree.h \
         $(INCDIR)/adt/uadt.h
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
	${RM} ${OBJ} ${BIN} $(RES) out.map
