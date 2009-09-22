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
CFLAGS  +=  -fopenmp
LDFLAGS +=  -lgomp -lpthread

#
OBJ = hello_gomp.o
BIN = hello_gomp.exe
RC  =
RES = ${RC:.rc=.res}

default: hello_gomp.exe

#
hello_gomp.exe : hello_gomp.o
	$(CC) -o $@ $< $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
hello_gomp.o : hello_gomp.c
	$(CC) -c -o $@ $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

