#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -luwin -luhook
#LIB = -L"../../lib" -mwindows -luwin

#
#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#	
SRC = test_ghook.cpp

#
OBJ = test_ghook.o
BIN = test_ghook.exe
RC  = ghook.rc
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
$(OBJ) : $(SRC) $(HEADER)
	$(CXX) -c -DBUILD_EXE $(SRC) $(CXXFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
	
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
