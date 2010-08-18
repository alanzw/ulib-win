#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS =
INCS += -I"$(TOPDIR)/inc" -I"." -DBUILD_EXE
LIBS += -L"$(TOPDIR)/lib"

#
OBJ = test_oledb.o
BIN = test_oledb.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
test_oledb.exe : test_oledb.o
	$(CXX) -o $@ $< -loleaut32

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
