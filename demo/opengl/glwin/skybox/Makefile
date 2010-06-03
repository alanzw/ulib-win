#
TOPDIR = ../../../..

include $(TOPDIR)/make.inc

#
JPEGLIB_INC = -I"$(TOPDIR)/../thirdparty-devel/include/jpeg"
JPEGLIB_LIB = -L"$(TOPDIR)/../thirdparty-devel/lib" -ljpeg

#
INCS = -I"$(INCDIR)" $(JPEGLIB_INC)
LIBS = -L"$(LIBDIR)" -mwindows -luwin -lopengl32 -lglu32  $(JPEGLIB_LIB)
CFLAGS += $(INCS)
CXXFLAGS += $(INCS) -DBUILD_EXE -DHU_DEBUG
LDFLAGS = $(LIBS)
#
OBJ = skybox_glwin.o \


BIN = skybox_glwin.exe
RC  = skybox.rc
RES = ${RC:.rc=.res}
#
HEADER = $(INCDIR)/ucontrol.h \
		 $(INCDIR)/udialogx.h \

#
all: $(BIN)

#
$(BIN) : $(OBJ) $(RES)
	$(CXX) -o $(BIN) $^ $(LIBS)
	cp -v $@ $(BINDIR)/$@

#
.PHONY : test
test :
	$(WINE) $(BINDIR)/$(BIN)
#
.PHONY : clean
clean :
	$(RM) ${OBJ} ${RES} ${BIN}

