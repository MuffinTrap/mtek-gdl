# This file is included by
# Makefile_linux, _mac and _win

# PC platform
CXXFILES	+= $(wildcard source/pc/*.cpp)
HDRS_PC		:= $(wildcard include/mgdl/pc/*.h)
OFILES		:= $(CXXFILES:.cpp=.pco)
HDRS		:= $(HDRS_X) $(HDRS_PC)

INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
ifdef DEBUG
CXXFLAGS += -g
CXXFLAGS += -DMGDL_DEBUG
else
CXXFLAGS += -O3
endif
CXXFLAGS += -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static

# Position independent code
CXXFLAGS += -fPIC

