# Makefile for building the Nintendo Wii (libogc) version of MTek-GDL
# Note: Only use MSys' make program for this makefile due to linux style file path conventions
# Version 26.1 edit by muffintrap: added portlibs include

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Enviroment variable DEVKITPRO not set. Use sudo -E to use user's enviroment variables. DevkitPro might not be installed")
endif

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Enviroment variable DEVKITPPC not set. DevkitPPC might not be installed")
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)g++
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

PORTLIBS_INC	:=	$(DEVKITPRO)/portlibs/ppc/include

INCLUDE	:= -Iinclude -I$(LIBOGC_INC) -I$(PORTLIBS_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Werror -Wall -Wextra -fsanitize=undefined -fsanitize=address $(MACHDEP) $(INCLUDE)

LIB 	:= mgdl
CFILES	:= $(wildcard source/wii/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib/wii/lib$(LIB).a
HDR 	:= $(wildcard include/mgdl-wii/*.h)

.PHONY: all clean install

all : $(OFILES)
	mkdir -p lib/wii
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r lib/wii

# Note: to install you need permissions to modify /opt/
# use the command $ sudo -E make install
# The -E flag uses the user's enviroment variables
install :
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/mgdl-wii/
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f include/mgdl-wii.h $(LIBOGC_INC)
	cp -f $(HDR) $(LIBOGC_INC)/mgdl-wii/

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
