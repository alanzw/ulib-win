#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" 

#
OBJ = test_sstorage.o
BIN = test_sstorage.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADERS =

#
all: $(BIN) 

#
test_sstorage.exe : test_sstorage.o
	$(CXX) -o $@ $< -loleaut32
  
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
