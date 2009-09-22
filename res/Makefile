#
include ../make.inc
#
SRCEXT = .cpp
OBJEXT = .o
BINEXT = .exe

#
CFLAGS   =
CXXFLAGS =
INC      =
LIB      =
LDFLAGS  =
#
SRCDIR = ../src
INCDIR = ../inc
OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
RESDIR = ../ico

RC  = u.rc version.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
.PHONY: all
all: $(RES)

#
%.res : %.rc
	$(WINDRES) -i $< -o $@ -DMINGW_RC -O coff -I"$(RESDIR)" -I"$(INCDIR)"
 
.PHONY: clean
clean:
	$(RM) $(RES)
