#
TOPDIR = ../..
#
include $(TOPDIR)/make.inc

#
INCS = -I"$(TOPDIR)/inc"
LIBS = -L"$(TOPDIR)/lib" -mwindows -luwin -lcomctl32
CFLAGS +=
CXXFLAGS += -DBUILD_EXE
#
OBJ = test_taskdialog.o

BIN = test_taskdialog.exe
RC  = taskdialog.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/umsg.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@
    
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

