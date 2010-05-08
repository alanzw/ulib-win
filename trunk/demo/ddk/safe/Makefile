#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS = -DHU_DEBUG -DBUILD_EXE
CXXFLAGS =
#LDFLAGS = -Wl,--subsystem=native,--image-base=0x10000,--file-alignment=0x1000,--section-alignment=0x1000,--entry=_DriverEntry@8 -nostartfiles -nostdlib -lntoskrnl
LDFLAGS = -s -nostartfiles -Wl,-nostdlib -shared -Wl,--entry,_DriverEntry@8 -Wl,--file-alignment,0x0080 -Wl,--section-alignment,0x0080 -Wl,--image-base,0x00010000 -lntoskrnl -lntstrsafe -lcsq
INC = -I"$(INCDIR)/inc"
LIB = -L"$(LIBDIR)/lib" -L"."

#
OBJ = safe.o
BIN = safe.sys
RC  =
RES = ${RC:.rc=.res}
#
HEADERS = safe.h precomp.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CC) -o $(BIN) $^ $(LIB) $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
