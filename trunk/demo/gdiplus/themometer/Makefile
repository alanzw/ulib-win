#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
LIBDIR = $(TOPDIR)/lib

#
GDIPLUS_DIR = D:\GS\mingw4\include\gdiplus

#
INC = -I"$(INCDIR)" -I"$(GDIPLUS_DIR)"
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lgdiplus
CFLAGS += 
CXXFLAGS += $(INC)
LDFLAGS += $(LIBS)
#
OBJ = themometer.o
BIN = themometer.exe
RC  = themometer.rc
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
themometer.o : themometer.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS)

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

