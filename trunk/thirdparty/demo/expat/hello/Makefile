#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
EXPAT_INC = $(THIRDPARTY_DIR)/include/expat
EXPAT_LIB = -L$(THIRDPARTY_DIR)/lib -lexpat
#
INC = -I"$(INCDIR)" -I"$(EXPAT_INC)"
LIBS = -L"$(LIBDIR)" $(EXPAT_LIB) -lshlwapi
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = expat_hello.o
BIN = expat_hello.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

