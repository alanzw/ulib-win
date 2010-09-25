#
TOPDIR = ..

#
include $(TOPDIR)/make.inc

#
RC  = u.rc version.rc
RES = ${RC:.rc=.res}
#
HEADER =
#
.PHONY: all
all: $(RES)

#
%.res : %.rc
	$(WINDRES) -i $< -o $@ -DMINGW_RC -O coff -I"$(RESDIR)" -I"$(ICODIR)"
 
.PHONY: clean
clean:
	$(RM) $(RES)
