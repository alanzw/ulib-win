#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
LUA_INC = $(THIRDPARTY_DIR)/include/lua
LUA_LIB = -L$(THIRDPARTY_DIR)/lib -llua -lopengl32 -lglu32
#
INC = -I"$(INCDIR)" -I"$(LUA_INC)"
LIBS = -L"$(LIBDIR)" $(LUA_LIB)
CFLAGS += 
CXXFLAGS += $(INC)
LDFLAGS += $(LIBS)
#
OBJ = lua_hello.o
BIN = lua_hello.exe
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

