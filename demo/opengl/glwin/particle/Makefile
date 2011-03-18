#
TOPDIR = ../../../..

include $(TOPDIR)/make.inc

#
INCS = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lopengl32 -lglu32
CFLAGS += $(INCS)
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS = $(LIBS)
#
OBJ = particle_glwin.o

BIN = particle_glwin.exe
RC  = particle.rc
RES = ${RC:.rc=.res}
#
HEADERS = 
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

