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
OBJ = test_odbc.o

BIN = test_odbc.exe
RC  = odbc.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h \
		 $(INCDIR)/ucontrol.h \
		 $(INCDIR)/uodbc.h \
		 $(INCDIR)/ubutton.h
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

