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
OBJ = test_button.o    \
      color_button.o   \

BIN = test_button.exe
RC  = button.rc
RES = ${RC:.rc=.res}
#
HEADERS = $(INCDIR)/ucontrol.h \
		  $(INCDIR)/ubutton.h  \
		  $(INCDIR)/udialog.h  \
		  round_button.h       \
          color_button.h       \
          cool_button.h        \
          imagebutton.h        \

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

