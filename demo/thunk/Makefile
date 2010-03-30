#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows

#	
SRC = test_thunk.cpp

#
OBJ = test_thunk.o
BIN = test_thunk.exe
RC  = thunk.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucombobox.h  
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $(OBJ) $(LIBS) $(RES)
	cp -v $@ $(BINDIR)/$@
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
	
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
