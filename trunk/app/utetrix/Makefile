#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_tetrix.o \
      utetrix.o     \
      ushape.o

BIN = test_tetrix.exe
RC  = tetrix.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ubasewindow.h \
         $(INCDIR)/uwinapp.h     \
         utetrix.h               \
         ushape.h                \

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

