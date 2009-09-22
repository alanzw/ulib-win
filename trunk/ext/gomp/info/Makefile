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
OBJ = info_gomp.o
BIN = info_gomp.exe
RC  =
RES = ${RC:.rc=.res}

default: info_gomp.exe

#
info_gomp.exe : info_gomp.o
	$(CC) -o $@ $< $(LDFLAGS)
	cp -v $@ $(BINDIR)/$@

#
info_gomp.o : info_gomp.c
	$(CC) -c -o $@ $< $(CFLAGS)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

