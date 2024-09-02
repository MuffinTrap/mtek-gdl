# Template project linux Makefile

SRC_DIRS := .

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')

# Setup compilation options
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# Extra compilation options
# #############################

# For Debugging
CXXFLAGS += -ggdb

# For optimization
# CXXFLAGS += -O3

## ROCKET module
## Add rocket files by uncommenting the 4 lines below
## The rocket code of library is compiled in SYNC_PLAYER mode

cpp_src += $(wildcard ../3rdparty/modules/rocket/*.cpp)
ROCKET_INCLUDE = -I../3rdparty/modules/rocket/
CXXFLAGS += $(ROCKET_INCLUDE)
CXX_FLAGS += -DSYNC_PLAYER

# ############################################
# Linux specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf

# Create a list of libraries that need to be linked
LDFLAGS = -lmgdl -lpng -lsndfile -lopenal -lGL -lGLU -lglut  -Wno-unused-function -z muldefs

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.lo)

# ########################
# Common settings and targets

# Add mgdl search directory and include
MGDL_DIR = $(HOME)/libmgdl
LDFLAGS += -L$(MGDL_DIR)
MGDL_INCLUDE = -I$(MGDL_DIR)

# Add them all to Compilation options
CXXFLAGS += $(MGDL_INCLUDE) 


.PHONY: all

all: $(TARGET)

# When all OFILES have been processed, link them together
$(TARGET) : $(OFILES)
	$(CXX) $(OFILES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

# Remove obj directory, all object files and the target
clean:
	@echo clean ...
	@rm -fr $(OBJ_DIR) $(OFILES) $(TARGET)

# For any .cpp file, create a object file with the same
# name.
# Create object directory and move all object files there
%.lo : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
