#
include ../../../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lopengl32 -lglu32

#
SRCDIR = ../../../../src
INCDIR = ../../../../inc
OBJDIR = ../../../../obj
RESDIR = ../../../../res
ICODIR = ../../../../ico
BINDIR = ../../../../bin
LIBDIR = ../../../../lib
#
OBJ = graph_glwin.o \
      uglgraph.o    \


BIN = graph_glwin.exe
RC  = graph.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/udialogx.h \
         uglgraph.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
%.o : %.cpp $(HEADER)
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

