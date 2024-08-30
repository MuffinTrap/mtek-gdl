# Template project windows Makefile

SRC_DIRS := .

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')

# Setup common compilation options
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# #############################
# Extra compilation options

# For Debugging
CXXFLAGS += -ggdb

# For optimization
# CXXFLAGS += -O3

# Rocket module
# Add rocket files to source
cpp_src += $(wildcard ../3rdparty/modules/rocket/*.cpp)
ROCKET_INCLUDE = -I../3rdparty/modules/rocket/
CXXFLAGS += $(ROCKET_INCLUDE)
# The rocket code of the release is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# ######################################
# Windows specific settings

# Set Compiler
CXX = g++
EXE_SUFFIX = .exe

# Create a list of libraries that need to be linked

# NOTE ! 
# Bloody C++ linking order is "last is first" and it does not read the
# list before it tries to link
# So the libraries not used by other libraries need to be last
# and mgdl needs to be first
LDFLAGS = -lmgdl -lpng -lopenal -lsndfile -lfreeglut -lglu32 -lopengl32 -lws2_32 -Wl,--allow-multiple-definition

# Executable is the same name as current directory +
# platform specific postfix
# Add numbers to the end if it crashes
# and you need to make a new one
TARGET	:=	$(notdir $(CURDIR))_win.exe

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.wo)

# END Windows specific

# ###################################
# Common settings and targets

# Add mgdl library search directory and include
MGDL_DIR = $(HOME)/libmgdl
LDFLAGS += -L$(MGDL_DIR)
MGDL_INCLUDE = -I$(MGDL_DIR)

# Add them all to Compilation options
CXXFLAGS += $(MGDL_INCLUDE)


.PHONY: all

all : $(TARGET)

# When all OFILES have been processed, link them together
$(TARGET) : $(OFILES)
	$(CXX) $(OFILES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) 

# Remove obj directory, all object files and the target
clean:
	@echo clean ...
	@rm -fr $(OFILES) $(TARGET)

# For any .cpp file, create a object file with the same
# name.
%.wo : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
