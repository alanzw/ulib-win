#
TOPDIR = ../..

#
include ${TOPDIR}/make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG -DBUILD_EXE
INCS = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -luwin -lpsapi

#
OBJ = test_process.o
BIN = test_process.exe
RC  = process.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uwinchar.h \
         $(INCDIR)/ulib.h
#
all: $(BIN) child.exe 

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

child.exe : child.o
	$(CXX) -o $@ $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@
    
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} child.o child.exe
