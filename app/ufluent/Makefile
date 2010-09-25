#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin -luhook
CFLAGS +=
CXXFLAGS +=  -DHU_DEBUG -DBUILD_EXE

#
OBJ =          \
 test_fluent.o \
 ufluent.o     \
 ufluentman.o  \
 ugambitman.o

BIN = test_fluent.exe
RC  = fluent.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h \
		 ufluent.h
#
all: $(BIN) fluent_reader.exe

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

fluent_reader.exe : ufluent.o fluent_reader.o $(RES)
	$(CXX) -o $@ $^
	cp -v $@ $(BINDIR)/$@   

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)     
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.o *.exe

