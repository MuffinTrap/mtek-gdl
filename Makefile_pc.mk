# This file is included by
# Makefile_linux, _mac and _win

# Library name
LIB 	:= mgdl
ARC 	:= lib$(LIB).a

# Own code
CXXFILES	= $(wildcard source/cross/*.cpp)
HDRS_X		:= $(wildcard include/mgdl/*.h)

# Embedded libraries
CXXFILES	+= $(wildcard include/mgdl/ufbx/*.cpp)
UFBX_HDR	:= include/mgdl/ufbx/ufbx.h
CCVEC_HDR	:= include/mgdl/ccVector/ccVector.h

# PC platform
CXXFILES	+= $(wildcard source/pc/*.cpp)
HDRS_PC		:= $(wildcard include/mgdl/pc/*.h)
OFILES		:= $(CXXFILES:.cpp=.pco)
HDRS		:= $(HDRS_X) $(HDRS_PC)
LIBHDR		:= include/mgdl.h

INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
CXXFLAGS = -O3 -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static
# UFBX settings
CXXFLAGS += -DUFBX_REAL_IS_FLOAT

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXXFLAGS += $(MGDL_INCLUDE)
