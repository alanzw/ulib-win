#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"  -mwindows -luwin
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS += $(LIBS)

#	
SRC = test_hook.cpp

#
OBJ = test_hook.o
BIN = test_hook.exe
RC  = hook.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucombobox.h  
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $(OBJ) $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
	
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
