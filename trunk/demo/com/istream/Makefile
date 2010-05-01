#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
CFLAGS +=
CXXFLAGS += -DBUILD_EXE
INCS = -I"$(TOPDIR)/inc" -I"."
LIBS = -L"$(TOPDIR)/lib" -luwin -luuid -lole32 -loleaut32 -mwindows 

#
OBJS = test_istream.o \
       ustream.o
BIN = test_istream.exe
RC  = istream.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
$(BIN) : $(OBJS) $(RES)
	$(CXX) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@

#
.PHONY : test
test:
	$(WINE) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
