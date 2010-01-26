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
OBJ = test_edit.o

BIN = test_edit.exe
RC  = edit.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h  \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/uedit.h    \
         ./ueditex.h
#
all: $(BIN) 

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp $@ $(BINDIR)/$@
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${BIN} $(RES)

