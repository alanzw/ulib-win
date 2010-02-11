#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -L"." -mwindows -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE
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
libskin.dll : libskin.o skin.rc
	$(WINDRES) -i skin.rc -o skin.res -O coff -I"$(ICODIR)" -I"$(RESDIR)"
	$(CXX) -shared -o libskin.dll libskin.o skin.res -Wl,--out-implib,libskin.a  $(LIBS)

#
libskin.o : skin.cpp skin.h
	$(CXX) -c -o $@ -DBUILD_DLL $< -DBUILD_DLL $(INCS)

#
.PHONY : test
test :
	$(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} libskin.dll libskin.a skin.res libskin.o
