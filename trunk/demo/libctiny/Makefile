#
TOPDIR = ../..

include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=  $(INCS) -v -O2 -fno-asm -fno-builtin -fno-builtin-function
CXXFLAGS += $(INCS) -fno-builtin-function
LDFLAGS += $(LIBS) -v -nostartfiles -nostdlib -lkernel32 -luser32
#
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/inc
OBJDIR = $(TOPDIR)/obj
RESDIR = $(TOPDIR)/res
ICODIR = $(TOPDIR)/ico
BINDIR = $(TOPDIR)/bin
#
OBJS = test_libctiny.o \
       dllcrt0.o     \
       initterm.o     \
       printf.o

BIN = test_libctiny.exe
RC  = libctiny.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : test_libctiny.o $(RES)
	$(CC) -o $(BIN) $^ $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
%.o : %.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJS} ${RES} ${BIN} *.o
