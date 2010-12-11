#
TOPDIR = ../../..

#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib"
CFLAGS +=
CXXFLAGS += -DBUILD_EXE

#
OBJ = webserver.o
BIN = webserver.exe
RC  = webserver.rc
RES = ${RC:.rc=.res}
#
HEADER =

#
all: $(BIN) 

#
webserver.exe: webserver.o helper.o servreq.o reqhead.o resphead.o resource.o
	gcc -o webserver webserver.o helper.o servreq.o reqhead.o resphead.o resource.o -Wall -lws2_32

webserver.o: webserver.c helper.h servreq.h
	gcc -o webserver.o webserver.c -c -ansi -pedantic -Wall

helper.o: helper.c helper.h
	gcc -o helper.o helper.c -c -ansi -pedantic -Wall

servreq.o: servreq.c servreq.h helper.h
	gcc -o servreq.o servreq.c -c -ansi -pedantic -Wall

reqhead.o: reqhead.c reqhead.h servreq.h helper.h
	gcc -o reqhead.o reqhead.c -c -ansi -pedantic -Wall

resphead.o: resphead.c resphead.h helper.h
	gcc -o resphead.o resphead.c -c -ansi -pedantic -Wall

resource.o: resource.c resource.h
	gcc -o resource.o resource.c -c -ansi -pedantic -Wall

.PHONY : clean
clean:
	$(RM) *.o *.exe *.res

