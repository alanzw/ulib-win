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
OBJ = chunk_gomp.o
BIN = chunk_gomp.exe
RC  =
RES = ${RC:.rc=.res}

default: chunk_gomp.exe

#
chunk_gomp.exe : chunk_gomp.o
	$(CC) -o $@ $< $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
chunk_gomp.o : chunk_gomp.c
	$(CC) -c -o $@ $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

