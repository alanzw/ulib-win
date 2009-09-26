#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIB = -L"../../lib" -mwindows -lcomctl32 -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = test_richedit$(OBJEXT)
BIN = test_richedit.exe
RC  = richedit.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/urichedit.h \
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@

#
test_richedit.o : test_richedit.cpp $(HEADER)
	$(CXX) -c -DWITHOUT_DLL -o $@ $< $(CXXFLAGS) $(INC)

$(OBJDIR)/udialog.o : $(SRCDIR)/udialog.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)
#
$(OBJDIR)/ucontrol.o : $(SRCDIR)/ucontrol.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)
#
$(OBJDIR)/urichedit.o : $(SRCDIR)/urichedit.cpp $(HEADER)
	$(CXX) -c -o $@ -DWITHOUT_DLL $< $(CXXFLAGS) $(INC)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
