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
OBJ = test_anime$(OBJEXT)
BIN = test_anime.exe
RC  = anime.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ucontrol.h \
         $(INCDIR)/uanimation.h \
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB) $(RES)
	cp -v $@ $(BINDIR)/$@
	cp -v 1.avi $(BINDIR)/1.avi

#
test_anime.o : test_anime.cpp $(HEADER)
	$(CXX) -c -DBUILD_EXE -o $@ $< $(CXXFLAGS) $(INC)

#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $< -o $@ -O coff -I"$(RESDIR)" -I"$(ICODIR)"
#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
