#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS = -DHU_DEBUG
CXXFLAGS =
#LDFLAGS = -Wl,--subsystem=native,--image-base=0x10000,--file-alignment=0x1000,--section-alignment=0x1000,--entry=_DriverEntry@8 -nostartfiles -nostdlib -lntoskrnl
LDFLAGS = -s -nostartfiles -Wl,-nostdlib -shared -Wl,--entry,_DriverEntry@8 \
          -Wl,--file-alignment,0x0080 -Wl,--section-alignment,0x0080        \
          -Wl,--image-base,0x00010000 -lntoskrnl

INCS = -I"$(INCDIR)"
LIBS = -L"$(LIBDIR)" -L"."

#
OBJ = ProcessSpy.o
BIN = ProcessSpy.sys
RC  =
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) test_ProcessSpy.exe ProcessSpy2.sys

#
$(BIN) : $(OBJ) $(RES)
	$(CC) -o $(BIN) $^ $(LIBS) $(LDFLAGS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
ProcessSpy.o : ProcessSpy.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS) $(INCS)
    
ProcessSpy2.sys : ProcessSpy2.o $(RES)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)
	cp -vf $@ $(BINDIR)/$@

#
ProcessSpy2.o : ProcessSpy2.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS) $(INCS)
    
test_ProcessSpy.exe : test_ProcessSpy.o $(RES)
	$(CC) -o $@ $^ $(LIBS)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

test_ProcessSpy.o : test_ProcessSpy.c $(HEADER)
	$(CC) -c -o $@ -DBUILD_EXE $< $(CFLAGS) $(INCS)

#
#.PHONY : test
#test :
#	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} *.o *.sys *.exe
