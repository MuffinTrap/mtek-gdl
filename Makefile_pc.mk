# This file is included by
# Makefile_linux, _mac and _win

include Makefile_common.mk

# PC platform
CXXFILES	+= $(wildcard source/pc/*.cpp)
HDRS_PC		:= $(wildcard include/mgdl/pc/*.h)
OFILES		:= $(CXXFILES:.cpp=.pco)
HDRS		:= $(HDRS_X) $(HDRS_PC)

INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
CXXFLAGS = -O3 -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static
# UFBX settings
CXXFLAGS += -DUFBX_REAL_IS_FLOAT
# Vector library
CXXFLAGS	+= -DMGDL_USE_CCVECTOR

# Position independent code
CXXFLAGS += -fPIC

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXXFLAGS += $(MGDL_INCLUDE)
