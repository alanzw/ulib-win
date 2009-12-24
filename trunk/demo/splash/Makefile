#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = test_splash$(OBJEXT)
BIN = test_splash.exe
RC  = splash.rc
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
test_splash.o : test_splash.cpp $(HEADER)
	$(CXX) -c -DBUILD_EXE -o $@ $< $(CXXFLAGS) $(INC)

#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $< -o $@ -DMINGW_RC -O coff -I"$(RESDIR)" -I"$(ICODIR)"
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean :
	${RM} ${OBJ} ${BIN} $(RES)
