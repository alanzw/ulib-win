#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
INC = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)"
CFLAGS += 
CXXFLAGS += $(INC) -DBUILD_EXE
LDFLAGS += $(LIBS)

#
OBJ = test_wowaccess.o
BIN = test_wowaccess.exe
RC  = wowaccess.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uwinchar.h \
         $(INCDIR)/ulib.h
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
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
