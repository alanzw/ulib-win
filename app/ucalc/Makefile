#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS =
INC = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
OBJ = test_calc.o \
      ucalc.o     \
      ueval.o     \

BIN = test_calc.exe
RC  = calc.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ubasewindow.h \
         $(INCDIR)/uwinapp.h     \


#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

.PHONY : test
test :
	$(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

