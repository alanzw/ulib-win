#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = test_splitter$(OBJEXT)
BIN = test_splitter.exe
RC  = splitter.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@

#
%.o : %.cpp $(HEADER)
	$(CXX) -c -DBUILD_EXE -o $@ $< $(CXXFLAGS) $(INC)

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
