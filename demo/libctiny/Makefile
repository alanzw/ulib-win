#
TOPDIR = ../..

include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=  $(INCS) -O2 -fno-asm -fno-builtin -fno-builtin-function
CXXFLAGS += $(INCS) -fno-builtin-function
LDFLAGS += $(LIBS) -nostartfiles -nostdlib -lkernel32 -luser32

#
OBJS = test_libctiny.o \
       dllcrt0.o       \
       initterm.o      \
       printf.o        \

BIN = test_libctiny.exe
RC  = libctiny.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN)

#
$(BIN) : test_libctiny.o $(RES) libtcrt.dll.a
	$(CC) -o $(BIN) $^ $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

libtcrt.dll.a : libtcrt.dll
libtcrt.dll : dllcrt0.o initterm.o printf.o
	$(CC) -shared -o $@ $^ -Wl,--output-def,libtcrt.def,--out-implib,libtcrt.dll.a $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
%.o : %.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS)

#
%.o : %.cpp $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJS} ${RES} ${BIN} *.o
