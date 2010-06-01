#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

THIRDPARTY_DIR = $(TOPDIR)/thirdparty
FREEGLUT_INC = $(THIRDPARTY_DIR)/include/freeglut
FREEGLUT_LIB = -L$(THIRDPARTY_DIR)/lib -lgdi32 -lwinmm -ldsound -lopengl32 -lglu32 -lfreeglut 
#
INCS = -I"$(INCDIR)" -I"$(FREEGLUT_INC)" -I"."
LIBS = -L"$(LIBDIR)" $(FREEGLUT_LIB)
CFLAGS += 
CXXFLAGS += $(INC)
LDFLAGS += $(LIBS)
#
OBJS = glut_shooter.o bitmap.o sound.o wave.o
BIN = glut_shooter.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJS) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJS} ${RES} ${BIN}

