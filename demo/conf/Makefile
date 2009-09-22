#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = -DHU_DEBUG
INCS = -I"../../inc"
LIBS = -L"../../lib" -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_conf.o
BIN = test_conf.exe
RC  = conf.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uconf.h \
         $(INCDIR)/ulib.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@
	cp -v default.cfg $(BINDIR)/default.cfg

#
test_conf.o : test_conf.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INCS)


#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
