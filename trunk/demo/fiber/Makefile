#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=
CXXFLAGS += $(INCS) -DBUILD_EXE
LDFLAGS += $(LIBS)
#
OBJ = test_fiber.o
BIN = test_fiber.exe
RC  = fiber.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uconf.h \
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
	$(WINE) $(BINDIR)/$(BIN) fiber.rc  fiber.rcc
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
