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
OBJ = dib_opengl.o

BIN = dib_opengl.exe
RC  = dib.rc
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
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

