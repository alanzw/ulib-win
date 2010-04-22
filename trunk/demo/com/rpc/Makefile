#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
CFLAGS =
CXXFLAGS = 
INCS = -I"$(TOPDIR)/inc" -I"." -DBUILD_EXE
LIBS = -L"$(TOPDIR)/lib" -lrpcrt4

#
OBJ = server.o client.o hello_rpc_s.o hello_rpc_c.o
BIN = server.exe client.exe
RC  = 
RES = ${RC:.rc=.res}
#
HEADER =
#
all: $(BIN) 

#
server.exe : server.o hello_rpc_s.o
	$(CC) -o $@ $^ -loleaut32 $(LIBS)
#
client.exe : client.o hello_rpc_c.o
	$(CC) -o $@ $^ -loleaut32	$(LIBS)
#
%.o : %.c
	$(CC) -c $< $(INCS)
    
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN}
