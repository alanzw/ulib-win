#
TOPDIR = ../../../..

include $(TOPDIR)/make.inc

#
INCS = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lopengl32 -lglu32
CFLAGS += $(INCS)
CXXFLAGS += $(INCS)
LDFLAGS = $(LIBS)
#
OBJ = bmp_opengl.o

BIN = bmp_opengl.exe
RC  = bmp.rc
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
bmp_opengl.o : bmp_opengl.cpp $(HEADER)
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

