#
include ../../make.inc

#
CFLAGS +=
CXXFLAGS += -DHU_DEBUG
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -lcomctl32 -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin

#
OBJ = \
 test_taskman.o \
 utaskman.o

BIN = test_taskman.exe
RC  = taskman.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h \
         utaskman.h \
         utabchild.h \
         uusagebar.h \
         uusagegraph.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@


#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

