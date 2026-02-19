# Makefile used by all PC platforms
MGDL_DIR	=$(HOME)/libmgdl

SRC_DIRS = .

# Setup compilation options
# Windows does not have address sanitizer
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# Add mgdl library search directory and include
MGDL_INCLUDE	= -I$(MGDL_DIR)/include
CXXFLAGS += $(MGDL_INCLUDE)

# Options for ufbx and vector library selection
CXXFLAGS += -DUFBX_REAL_IS_FLOAT
CXXFLAGS += -DMGDL_USE_CCVECTOR

# Optional: Add rocket files
SRC_DIRS += $(MGDL_DIR)/include/mgdl/rocket
ROCKET_INCLUDE = -I$(MGDL_DIR)/include/mgdl/rocket
CXXFLAGS += $(ROCKET_INCLUDE)

# CXXFLAGS += -DMGDL_ROCKET
# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')


# Extra compilation options
# #############################

# For Debugging
CXXFLAGS += -ggdb


# ##########################
# RELEASE BUILD
############################
# For optimization
# CXXFLAGS += -O3

# The rocket code of the release is compiled in SYNC_PLAYER mode
# CXXFLAGS += -DSYNC_PLAYER
