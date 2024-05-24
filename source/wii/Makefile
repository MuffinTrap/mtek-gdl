ifeq ($(strip $(DEVKITPRO)),)
  $(error "DevkitPRO may not be installed")
endif

ifeq ($(strip $(DEVKITPPC)),)
  $(error "DevkitPPC may not be installed")
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)g++
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

INCLUDE	:= -I../../include -I$(LIBOGC_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Werror -Wall -Wextra -fsanitize=undefined,address$(MACHDEP) $(INCLUDE)

LIB 	:= mgdl
CFILES	:= $(wildcard *.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(wildcard ../../include/mgdl-wii/*.h)

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/mgdl-wii/
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f ../../include/mgdl-wii.h $(LIBOGC_INC)
	cp -f $(HDR) $(LIBOGC_INC)/mgdl-wii/

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
