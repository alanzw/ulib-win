#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc


INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -lopengl32 -lglu32 -lglut32
#
CFLAGS += $(INCS) -D_STDCALL_SUPPORTED -D_M_IX86 -DGLUT_DISABLE_ATEXIT_HACK
CXXFLAGS += $(INCS) -D_STDCALL_SUPPORTED -D_M_IX86 -DGLUT_DISABLE_ATEXIT_HACK
#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
OBJ = test_glsurf.o \

BIN = test_glsurf.exe
RC  = glsurf.rc
RES = ${RC:.rc=.res}
#
HEADER =


#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS)

.PHONY : test
test :
	$(BINDIR)/$(BIN) glsurf.dat
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

