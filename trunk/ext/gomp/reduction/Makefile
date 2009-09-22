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
OBJ = reduction_gomp.o
BIN = reduction_gomp.exe
RC  =
RES = ${RC:.rc=.res}

default: reduction_gomp.exe

#
reduction_gomp.exe : reduction_gomp.o
	$(CC) -o $@ $< $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
reduction_gomp.o : reduction_gomp.c
	$(CC) -c -o $@ $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

