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
BIN = test_db.exe db.dll
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

db.dll : db.o
	$(CXX) -shared -o $@ $^ -Wl,--output-def,libdb.def,--out-implib,libdb.dll.a,--kill-at
#
test_db.exe : test_db.o db.dll
	$(CXX) -o $@ $< -L"." -lole32 

db.o : db.cpp db.h
	$(CXX) -fPIC -c -o $@ $< -DBUILD_DLL $(INCS)

clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.a *.dll *.def
