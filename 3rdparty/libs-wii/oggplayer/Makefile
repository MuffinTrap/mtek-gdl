# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PORTLIBS_INC	:=	$(DEVKITPRO)/portlibs/ppc/include

INCLUDE	:= -I$(LIBOGC_INC) -I$(PORTLIBS_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= oggplayer
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
