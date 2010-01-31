#
TOPDIR = ../../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -lws2_32 -lcomctl32 -lgdi32 -mwindows
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJS = httpui.o httpmt.o countc.c
BIN = httpmt_sock.exe
RC  = httpmt.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
httpmt_sock.exe : $(OBJS) $(RES)
	$(CC) -o $@ $^ $(LIBS)
	cp -vf $@ $(BINDIR)/$@

%.o : %.c
	$(CC) -c -o $@ $^ $(INCS)
    
#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
