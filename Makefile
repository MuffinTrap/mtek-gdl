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

PORTLIBS_PPC	:=	$(DEVKITPRO)/portlibs/ppc/include
PORTLIBS_WII	:=	$(DEVKITPRO)/portlibs/wii/include

ROCKET_INC	:=	3rdparty/libs-cross/

INCLUDE	:= -Iinclude -I$(LIBOGC_INC) -I$(PORTLIBS_PPC) -I$(PORTLIBS_WII) -I$(ROCKET_INC)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
# SYNC_PLAYER is for rocket
CFLAGS  := -O3 -Werror -Wall -Wextra $(MACHDEP) $(INCLUDE) -DSYNC_PLAYER -DGEKKO

LIB 	:= mgdl
CFILES	:= $(wildcard source/wii/*.cpp)
CFILES	+= $(wildcard source/cross/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib/wii/lib$(LIB).a
HDRS_X 	:= $(wildcard include/mgdl/*.h)
HDRS_WII	:= $(wildcard include/mgdl-wii/*.h)
HDRS := $(HDRS_X) $(HDRS_WII)
HDR 	:= $(wildcard include/mgdl-wii/*.h)
HDR 	+= $(wildcard include/mgdl/*.h)

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
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/mgdl
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/mgdl-wii
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f include/mgdl.h $(LIBOGC_INC)
	cp -f $(HDRS_X) $(LIBOGC_INC)/mgdl
	cp -f $(HDRS_WII) $(LIBOGC_INC)/mgdl-wii

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
