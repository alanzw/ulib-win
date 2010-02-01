#
TOPDIR = ../../../..

#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = db.o test_db.o
BIN = test_db.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
test_db.exe : db.o test_db.o
	$(CXX) -o $@ $^
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
