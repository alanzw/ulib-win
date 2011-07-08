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
	gcc -g -o webserver webserver.o helper.o servreq.o reqhead.o resphead.o resource.o -Wall -lws2_32
	cp -vf $@ $(BINDIR)/$@

webserver.o: webserver.c helper.h servreq.h
	gcc -g -o webserver.o webserver.c -c -ansi -pedantic -Wall

helper.o: helper.c helper.h
	gcc -g -o helper.o helper.c -c -ansi -pedantic -Wall

servreq.o: servreq.c servreq.h helper.h
	gcc -g -o servreq.o servreq.c -c -ansi -pedantic -Wall

reqhead.o: reqhead.c reqhead.h servreq.h helper.h
	gcc -g -o reqhead.o reqhead.c -c -ansi -pedantic -Wall

resphead.o: resphead.c resphead.h helper.h
	gcc -g -o resphead.o resphead.c -c -ansi -pedantic -Wall

resource.o: resource.c resource.h
	gcc -g -o resource.o resource.c -c -ansi -pedantic -Wall

.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)

.PHONY : clean
clean:
	$(RM) *.o *.exe *.res

