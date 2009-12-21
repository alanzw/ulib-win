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
OBJ = test_icon.o \
      uxicon.o

BIN = test_icon.exe
RC  = icon.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/udialogx.h \
         uxicon.h
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
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

