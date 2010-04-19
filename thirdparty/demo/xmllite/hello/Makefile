#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
XMLLITE_INC = $(THIRDPARTY_DIR)/include/xmllite
XMLLITE_LIB = -L$(THIRDPARTY_DIR)/lib -lxmllite
#
INC = -I"$(INCDIR)" -I"$(XMLLITE_INC)"
LIBS = -L"$(LIBDIR)" $(XMLLITE_LIB) -lshlwapi
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = xmllite_hello.o
BIN = xmllite_hello.exe
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

