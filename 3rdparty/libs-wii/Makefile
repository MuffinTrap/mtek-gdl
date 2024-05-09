LIBOGC_INC	:= $(DEVKITPRO)/libogc/include
LIBOGC_LIB	:= $(DEVKITPRO)/libogc/lib/wii
PORTLIBS_INC	:=	$(DEVKITPRO)/portlibs/ppc/include
PORTLIBS_LIB	:=	$(DEVKITPRO)/portlibs/ppc/lib

SUBDIRS		:= `ls | egrep -v '^(\.svn)$$'`

all:
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    echo "-----------------------------------------------------------";\
	    $(MAKE) -C $$i || { exit 1;} \
	  fi; \
	done;

clean:
	@rm -f *.bz2
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    $(MAKE) -C $$i clean || { exit 1;} \
	  fi; \
	done;

install:
	@rm -f *.bz2
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    $(MAKE) -C $$i install || { exit 1;} \
	  fi; \
	done;