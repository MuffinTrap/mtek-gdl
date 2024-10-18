
# Template project linux Makefile

SRC_DIRS := .

# Use only CXX to compile all files

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')

# Setup compilation options
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# Extra compilation options
# #############################

# For Debugging
CXXFLAGS += -ggdb

CXXFLAGS += -DUFBX_REAL_IS_FLOAT

# For optimization
# CXXFLAGS += -O3

# ROCKET module

# Add rocket files
cpp_src += $(wildcard rocket/*.cpp)
ROCKET_INCLUDE = -Irocket/
DATA_INCLUDE = -Ibuild/
CXXFLAGS += $(ROCKET_INCLUDE)
CXXFLAGS += $(DATA_INCLUDE)
# The rocket code of library is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# ############################################
# Linux specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf

# Create a list of libraries that need to be linked
LDFLAGS = -lmgdl -lpng -lsndfile -lopenal -lGL -lGLU -lglut -Wno-unused-function -z muldefs

# Add mgdl library search directory and include
MGDL_DIR=$(HOME)/libmgdl
MGDL_DIR2=$(HOME)/libmgdl/mgdl

LDFLAGS += -L$(MGDL_DIR)
LDFLAGS += -L$(MGDL_DIR2)

MGDL_INCLUDE = -I$(MGDL_DIR) -I$(MGDL_DIR2)


# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

# ########################
# Common settings and targets

# Add them all to Compilation options
CXXFLAGS += $(MGDL_INCLUDE)

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.lo)

# Add binary files from build/
OFILES += $(wildcard build/*o)

.PHONY: all

# When all OFILES have been processed, link them together
all : $(OFILES)
	$(CXX) $(OFILES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

# Remove obj directory, all object files and the target
clean:
	@echo clean ...
	@rm -fr $(OFILES) $(TARGET)

# For any .cpp file, create a object file with the same
# name.
# Create object directory and move all object files there
%.lo : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
