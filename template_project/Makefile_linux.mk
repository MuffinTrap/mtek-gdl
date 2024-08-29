
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

# For optimization
# CXXFLAGS += -O3

# ROCKET module

# Add rocket files
cpp_src += $(wildcard ../3rdparty/modules/rocket/*.cpp)
ROCKET_INCLUDE = -I../3rdparty/modules/rocket/
CXXFLAGS += $(ROCKET_INCLUDE)
# The rocket code of library is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# ############################################
# Linux specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf
OBJ_DIR := obj_lnx

# Create a list of libraries that need to be linked
LDFLAGS = -lpng -lsndfile -lopenal -lGL -lGLU -lglut -lmgdl -Wno-unused-function -z muldefs

# Add mgdl search directory and include
LDFLAGS += -L../lib/lnx
MGDL_INCLUDE = -I../include/

# Add include directories for libraries
GLUT_INCLUDE = -I/usr/include/GL/

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

# ########################
# Common settings and targets

# Add them all to Compilation options
CXXFLAGS += $(GLUT_INCLUDE) $(MGDL_INCLUDE) 

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.o)

.PHONY: all

# Select all object files in OBJ_DIR
all : OBJ_FILES = $(wildcard $(OBJ_DIR)/*.o)

# When all OFILES have been processed, link them together
all : $(OFILES)
	$(CXX) $(OBJ_FILES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

# Remove obj directory, all object files and the target
clean:
	@echo clean ...
	@rm -fr $(OBJ_DIR) $(OFILES) $(TARGET)

# For any .cpp file, create a object file with the same
# name.
# Create object directory and move all object files there
%.o : %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	mv $@ $(OBJ_DIR)
