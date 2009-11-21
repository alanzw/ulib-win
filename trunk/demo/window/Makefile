#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -lmsimg32
CFLAGS +=
CXXFLAGS += -DWITHOUT_EXE


#
OBJ = test_window.o  \
      uwindow.o
BIN = test_window.exe
RC  = window.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uwindow.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@

uwindow.o : $(SRCDIR)/uwindow.cpp $(HEADER)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCS)

.PHONY: test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

