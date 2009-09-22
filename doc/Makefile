#
include ../make.inc

#
SRCDIR = ../src
INCDIR = ../inc
OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib
RESDIR = ../res

DEMODIR = ../demo
DOCDIR  = ../doc

#
all : doc

doc : Doxyfile
	doxygen $<

.PHONY : clean
clean :
	$(RM) -r html
	$(RM) -r latex

