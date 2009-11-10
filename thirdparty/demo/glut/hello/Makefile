#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
LIBDIR = $(TOPDIR)/lib

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
FREEGLUT_INC = $(THIRDPARTY_DIR)/include/freeglut
FREEGLUT_LIB = -L$(THIRDPARTY_DIR)/lib -lfreeglut -lopengl32 -lglu32
#
INC = -I"$(INCDIR)" -I"$(FREEGLUT_INC)"
LIBS = -L"$(LIBDIR)" $(FREEGLUT_LIB)
CFLAGS += 
CXXFLAGS += $(INC)
LDFLAGS += $(LIBS)
#
OBJ = glut_hello.o
BIN = glut_hello.exe
RC  =
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
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

