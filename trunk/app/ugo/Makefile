#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin -lwinmm
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_go.o \
      ugo.o     \
      ugo_kernel.o \


BIN = test_life.exe
RC  = go.rc
RES = ${RC:.rc=.res}
#
HEADERS = $(INCDIR)/ubasewindow.h \
         $(INCDIR)/uwinapp.h     \
         ugo.h                   \
         ugo_kernel.h            \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.o *.exe

