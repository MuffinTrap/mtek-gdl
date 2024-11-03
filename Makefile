# Makefile for building the Nintendo Wii (libogc) version of MTek-GDL
# Note: Only use MSys' make program for this makefile due to linux style file path conventions
# Version 26.1 edit by muffintrap: added portlibs include

# Library name
LIB 	:= mgdl
LIBDIR	:= lib/wii
ARC 	:= lib$(LIB).a

# Own code
CXXFILES	= $(wildcard source/cross/*.cpp)
HDRS_X		:= $(wildcard include/mgdl/*.h)

# Embedded libraries
CXXFILES	+= $(wildcard include/mgdl/ufbx/*.cpp)
UFBX_HDR	:= include/mgdl/ufbx/ufbx.h

# Wii specific settings
CXXFILES	+= $(wildcard source/wii/*.cpp)
HDRS_WII	:= $(wildcard include/mgdl/wii/*.h)
OFILES		:= $(CXXFILES:.cpp=.o)
HDRS		:= $(HDRS_X) $(HDRS_WII)
LIBHDR		:= include/mgdl.h

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Enviroment variable DEVKITPRO not set. Use sudo -E to use user's enviroment variables. DevkitPro might not be installed")
endif

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Enviroment variable DEVKITPPC not set. DevkitPPC might not be installed")
endif

PREFIX		:= $(DEVKITPPC)/bin/powerpc-eabi-
CXX		:= $(PREFIX)g++
AR		:= $(PREFIX)ar

# OGC and Portlibs includes and libraries for Wii
LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

PORTLIBS_PPC	:=	$(DEVKITPRO)/portlibs/ppc/include
PORTLIBS_WII	:=	$(DEVKITPRO)/portlibs/wii/include

OGC_INCLUDE	:= -I$(LIBOGC_INC) -I$(PORTLIBS_PPC) -I$(PORTLIBS_WII)
MACHDEP		:= -mrvl -mcpu=750 -meabi -mhard-float
# NOTE: Cannot use -Wpedantic since OGC is C
CXXFLAGS	= -O3 -Werror -Wall -Wextra -std=c++11 $(MACHDEP) -DGEKKO
# UFBX settings
CXXFLAGS	+= -DUFBX_REAL_IS_FLOAT

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXXFLAGS += $(MGDL_INCLUDE)
# NOTE: The order matters. Add OGC last so that includes are
# searched first from local include/
CXXFLAGS += $(OGC_INCLUDE)

INSTALL_DIR	= $(LIBOGC_INC)
# Targets

.PHONY: all clean install

all : $(OFILES)
	mkdir -p $(LIBDIR)
	$(AR) -r $(LIBDIR)/$(ARC) $(OFILES)

# Note: to install you need permissions to modify /opt/
# use the command $ sudo -E make install
# The -E flag uses the user's enviroment variables
install :
	# Create lib/wii/ and include/mgdl/
	mkdir -p $(LIBOGC_LIB) $(INSTALL_DIR)/$(LIB)
	# Subdir wii for wii headers
	mkdir -p $(INSTALL_DIR)/$(LIB)/wii

	cp -f $(LIBDIR)/$(ARC) $(LIBOGC_LIB)/
	# Main header to include/
	cp -f $(LIBHDR) $(INSTALL_DIR)
	# Cross platform headers to include/mgdl/
	cp -f $(HDRS_X) $(INSTALL_DIR)/$(LIB)
	# Wii headers to include/mgdl/wii/
	cp -f $(HDRS_WII) $(INSTALL_DIR)/$(LIB)/wii

# UFBX library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/ufbx
	@cp $(UFBX_HDR) $(INSTALL_DIR)/$(LIB)/ufbx

	@echo Library installed

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
