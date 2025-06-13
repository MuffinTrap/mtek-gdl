# Makefile used by all PC platforms

SRC_DIRS := .

# Use only CXX to compile all files

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')

# Setup compilation options
# Windows does not have address sanitizer
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# Extra compilation options
# #############################

# For Debugging
CXXFLAGS += -ggdb

CXXFLAGS += -DUFBX_REAL_IS_FLOAT
CXXFLAGS += -DMGDL_USE_CCVECTOR


# Rocket module
# Add rocket files to source
ROCKET_INCLUDE = -Irocket/
CXXFLAGS += $(ROCKET_INCLUDE)

# Add mgdl library search directory and include
# Need to have mgdl as well because ufbx
MGDL_DIR	=$(HOME)/libmgdl
MGDL_INCLUDE	= -I$(MGDL_DIR)
CXXFLAGS += $(MGDL_INCLUDE)

# ##########################
# RELEASE BUILD
############################
# For optimization
# CXXFLAGS += -O3

# The rocket code of the release is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER
