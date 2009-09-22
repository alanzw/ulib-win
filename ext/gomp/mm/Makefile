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
OBJ = mm_gomp.o
BIN = mm_gomp.exe
RC  =
RES = ${RC:.rc=.res}

default: mm_gomp.exe

#
mm_gomp.exe : mm_gomp.o
	$(CC) -o $@ $< $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
mm_gomp.o : mm_gomp.c
	$(CC) -c -o $@ $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

