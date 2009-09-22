#
include ../../make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"../../inc"
LIBS = -L"../../lib" -L. -mwindows -lcomctl32 -luwin

#
SRCDIR = ../../src
INCDIR = ../../inc
OBJDIR = ../../obj
RESDIR = ../../res
ICODIR = ../../ico
BINDIR = ../../bin
#
OBJ = test_skindlg.o
BIN = test_skindlg.exe
RC  = skindlg.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialog.h \
         $(INCDIR)/ubutton.h \
         $(INCDIR)/ucontrol.h
#
all: $(BIN) 

#
$(BIN) : $(OBJ) $(RES) libskin.dll
	$(CXX) -o $(BIN) $(OBJ) $(LIBS) $(RES) -lskin
	cp -v $@ libskin.dll $(BINDIR)

#
test_skindlg.o : test_skindlg.cpp $(HEADER)
	$(CXX) -c -DBUILD_EXE $< $(CXXFLAGS) $(INCS)
    
#
libskin.dll : libskin.o skin.rc
	$(WINDRES) -i skin.rc -o skin.res -O coff -I"$(ICODIR)" -I"$(RESDIR)"
	$(CXX) -shared -o libskin.dll libskin.o skin.res -Wl,--out-implib,libskin.a  $(LIBS)

#
libskin.o : skin.cpp skin.h
	$(CXX) -c -o $@ -DBUILD_DLL $< $(CXXFLAGS) $(INCS)
#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
.PHONY : test
test :
	$(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} libskin.dll libskin.a skin.res libskin.o
