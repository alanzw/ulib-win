#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -luwin -lcomctl32

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_draglistbox.o

BIN = test_draglistbox.exe
RC  = draglistbox.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/ulistbox.h \
		 $(INCDIR)/udialogx.h \
		 $(INCDIR)/udraglistbox.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
test_draglistbox.o : test_draglistbox.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
$(RES) : $(RC) resource.h
	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"
    
#
.PHONY : test
test:
	cd $(BINDIR) && $(WINE) ./$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

