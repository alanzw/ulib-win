#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
LUA_INC = $(THIRDPARTY_DIR)/include/lua
LUA_LIB = -L$(THIRDPARTY_DIR)/lib -llua
#
INC = -I"$(INCDIR)" -I"$(LUA_INC)"
LIBS = -L"$(LIBDIR)" $(LUA_LIB)
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS) -O2 -mno-cygwin -shared -Wl,-e,_DllMain@12 -Wl,--dll  -luser32 -lkernel32  -luwin
#
OBJ = lua_cxlib.o
BIN = lua_cxlib.dll
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

