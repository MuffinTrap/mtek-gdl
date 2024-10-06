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

INCLUDE	:= -Iinclude -I$(LIBOGC_INC) -I$(PORTLIBS_PPC) -I$(PORTLIBS_WII)
MACHDEP := -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O3 -Werror -Wall -Wextra $(MACHDEP) $(INCLUDE) -DGEKKO -DUFBX_REAL_IS_FLOAT

LIB 	:= mgdl
CFILES	:= $(wildcard source/wii/*.cpp)
CFILES	+= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard include/ufbx/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib/wii/lib$(LIB).a
HDR_W 	:= $(wildcard include/mgdl/wii/*.h)
HDR_X 	:= $(wildcard include/mgdl/*.h)

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
	# Create lib/wii/ and include/mgdl/
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/$(LIB)
	# Subdir wii for wii headers
	mkdir -p $(LIBOGC_INC)/$(LIB)/wii

	cp -f $(ARC) $(LIBOGC_LIB)/
	# Main header to include/
	cp -f include/mgdl.h $(LIBOGC_INC)
	# Cross platform headers to include/mgdl/
	cp -f $(HDR_X) $(LIBOGC_INC)/$(LIB)
	# Wii headers to include/mgdl/wii/
	cp -f $(HDR_W) $(LIBOGC_INC)/$(LIB)/wii

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
