#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

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
OBJ = glut_teapot.o
BIN = glut_teapot.exe
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

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

