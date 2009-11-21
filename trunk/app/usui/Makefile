#
TOPDIR = ../..

include $(TOPDIR)/make.inc

#
THIRDPARTY_DIR = $(TOPDIR)/thirdparty
THIRDPARTY_INC = $(THIRDPARTY_DIR)/include/lua
THIRDPARTY_LIB = -L$(THIRDPARTY_DIR)/lib -llua

#
INCS = -I"$(TOPDIR)/inc" -I$(THIRDPARTY_INC)
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin $(THIRDPARTY_LIB)
CFLAGS +=
CXXFLAGS += $(INCS)
LDFLAGS += $(LIBS)

#
OBJ = test_sui.o \
      usui.o     \
      ulua.o     \


BIN = test_sui.exe
RC  = sui.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ubasewindow.h \
         $(INCDIR)/uwinapp.h     \
         usui.h                  \
         ulua.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@
	cp -v $(THIRDPARTY_DIR)/bin/lua51.dll $(BINDIR)/lua51.dll

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

