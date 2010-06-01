#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
FREEGLUT_INC = $(THIRDPARTY_DIR)/include/freeglut
FREEGLUT_LIB = -L$(THIRDPARTY_DIR)/lib -lopengl32 -lglu32 -lfreeglut 
#
INCS = -I"$(INCDIR)" -I"$(FREEGLUT_INC)" -I"."
LIBS = -L"$(LIBDIR)" $(FREEGLUT_LIB)
CFLAGS += 
CXXFLAGS += $(INC)
LDFLAGS += $(LIBS)
#
OBJ = glut_dinoshade.o
BIN = glut_dinoshade.exe
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

