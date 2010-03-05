#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = -DBUILD_EXE 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib"

#
OBJ = test_ado.o
BIN = test_ado.exe
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
test_ado.exe : test_ado.o
	$(CXX) -o $@ $<

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
