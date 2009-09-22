#
include ../../../../make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"../../../../inc"
LIBS = -L"../../../../lib" -mwindows -lopengl32 -lglu32 -luwin

#
SRCDIR = ../../../../src
INCDIR = ../../../../inc
OBJDIR = ../../../../obj
RESDIR = ../../../../res
ICODIR = ../../../../ico
BINDIR = ../../../../bin
#
OBJ = mesh2d_opengl.o

BIN = mesh2d_opengl.exe
RC  = mesh2d.rc
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
mesh2d_opengl.o : mesh2d_opengl.cpp $(HEADER)
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

