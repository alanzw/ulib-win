#
TOPDIR = ../..

#
include $(TOPDIR)/make.inc

#
INC = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin
CFLAGS +=
CXXFLAGS += 

#
OBJ = test_sort.o \
      usortctrl.o \


BIN = test_sort.exe
RC  = sort.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ubasewindow.h \
         $(INCDIR)/uwinapp.h     \
         usortctrl.h
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

