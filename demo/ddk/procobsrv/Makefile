#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin

#
CFLAGS = -DHU_DEBUG
CXXFLAGS =
#LDFLAGS = -Wl,--subsystem=native,--image-base=0x10000,--file-alignment=0x1000,--section-alignment=0x1000,--entry=_DriverEntry@8 -nostartfiles -nostdlib -lntoskrnl
LDFLAGS = -s -nostartfiles -Wl,-nostdlib -shared -Wl,--entry,_DriverEntry@8 \
          -Wl,--file-alignment,0x0080 -Wl,--section-alignment,0x0080        \
          -Wl,--image-base,0x00010000 -lntoskrnl

INC = -I"$(INCDIR)/inc"
LIB = -L"$(LIBDIR)/lib"

#
OBJ = procobsrv.o
BIN = procobsrv.sys
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CC) -o $(BIN) $^ $(LIB) $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
procobsrv.o : procobsrv.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS) $(INC)

#
#$(RES) : $(RC) resource.h
#	$(WINDRES) -i $(RC) -o $(RES) -O coff -I"$(ICODIR)" -I"$(RESDIR)"

#
#.PHONY : test
#test :
#	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
