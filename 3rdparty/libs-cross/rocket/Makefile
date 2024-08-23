# This Makefile does not compile anything
# it simply copies the headers over to $(DEVKITPRO)/libogc/include/rocket

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif


LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include

INCLUDE	:= -I$(LIBOGC_INC) -I$(PORTLIBS_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= rocket
LIBDIR	:= rocket/
HFILES	:= $(wildcard *.h)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h

.PHONY: install

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/$(LIBDIR)
	cp -f $(HFILES) $(LIBOGC_INC)/$(LIBDIR)
