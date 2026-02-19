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

INSTALL_INC	:=	$(DEVKITPRO)/../libmgdl/include/dolexec
INSTALL_LIB	:=	$(DEVKITPRO)/../libmgdl/lib

INCLUDE	:= -I$(LIBOGC_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Wall $(MACHDEP) $(INCLUDE)

LIB 	:= dolexec
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(INSTALL_LIB)  $(INSTALL_INC)
	cp -f $(ARC) $(INSTALL_LIB)/
	cp -f $(HDR) $(INSTALL_INC)/

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
