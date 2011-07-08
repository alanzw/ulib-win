#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
SQLITE_INC = $(THIRDPARTY_DIR)/include/sqlite
SQLITE_LIB = -L$(THIRDPARTY_DIR)/lib -lsqlite3
#
INC = -I"$(INCDIR)" -I"$(SQLITE_INC)"
LIBS = -L"$(LIBDIR)" $(SQLITE_LIB)
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = sqlite_hello.o
BIN = sqlite_hello.exe
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

