#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
GDIPLUS_DIR = D:\GS\mingw4\include\gdiplus

#
INC = -I"$(INCDIR)" -I"$(GDIPLUS_DIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lgdiplus
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = button_gdiplus.o
BIN = button_gdiplus.exe
RC  = gdiplus.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/udialogx.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

