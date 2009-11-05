#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INC = -I"../../inc"
LIBS = -L"../../lib" -mwindows -lcomctl32 -luwin -L"." -lapihook

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_apihook.o

BIN = test_apihook.exe
RC  = apihook.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
%.o : %.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

apihook.o : apihook.cpp
	$(CXX) -fPIC -c -DBUILD_DLL -o $@ $< $(INC) $(CXXFLAGS)
    
apihook.dll : apihook.o
	$(CXX) -shared -o $@ $^ -Wl,--output-def,libapihook.def,--out-implib,libapihook.dll.a
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.dll *.def *.a *.o

