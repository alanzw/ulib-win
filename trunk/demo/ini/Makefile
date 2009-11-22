 -lcomctl32
#
OBJ = test_ini.o
BIN = test_ini.exe
RC  = ini.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/uini.h
#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIB)
	cp -vf $(BIN) $(BINDIR)/$(BIN)

#
test_ini.o : test_ini.cpp $(HEADER)
	$(CXX) -c -o $@ -DBUILD_EXE $< $(CXXFLAGS) $(INC)

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
clean :
	$(RM) ${OBJ} ${RES} ${BIN} new_test.ini
