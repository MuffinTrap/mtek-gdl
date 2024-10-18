# This file is included by
# Makefile_linux, _mac and _win

LIB 	:= mgdl

# Own code
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)

# Embedded libraries
CFILES	+= $(wildcard include/ufbx/*.cpp)

ARC 	:= lib$(LIB).a
HDRS_X 	:= $(wildcard include/mgdl/*.h)
HDRS_PC	:= $(wildcard include/mgdl/pc/*.h)
HDRS := $(HDRS_X) $(HDRS_PC)
PCHDR 	:= include/mgdl.h
INSTALL_DIR = $(HOME)/libmgdl

UFBX_HDR := include/ufbx/ufbx.h

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static
# UFBX settings
CXX_FLAGS += -DUFBX_REAL_IS_FLOAT

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
UFBX_INCLUDE = -Iinclude/mgdl/
CXX_FLAGS += $(MGDL_INCLUDE)
CXX_FLAGS += $(UFBX_INCLUDE)
