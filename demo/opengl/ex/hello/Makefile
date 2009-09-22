#
include ../../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin

#
SRCDIR = ../../../../src
INCDIR = ../../../../inc
OBJDIR = ../../../../obj
RESDIR = ../../../../res
ICODIR = ../../../../ico
BINDIR = ../../../../bin
LIBDIR = ../../../../lib
#
OBJ = hello_opengl.o

BIN = hello_opengl.exe
RC  = hello.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/udialogx.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
hello_opengl.o : hello_opengl.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

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

