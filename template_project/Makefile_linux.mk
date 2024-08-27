
# Template project linux Makefile

SRC_DIRS := .
OBJ_DIR := obj_lnx

# Use only CXX to compile all files

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')

# Add rocket files
cpp_src += $(wildcard ../3rdparty/modules/rocket/*.cpp)

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.o)

# Setup compilation options
CXXFLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# Extra compilation options
# #############################

# For Debugging
CXXFLAGS += -ggdb

# The rocket code of library is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# Linux specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf

# Create a list of libraries that need to be linked
LDFLAGS = -lpng -lsndfile -lopenal -lGL -lGLU -lglut -lmgdl -Wno-unused-function -z muldefs

# Add library search directories
LDFLAGS += -L../lib/lnx

# Add include directories for libraries
GLUT_INCLUDE = -I/usr/include/GL/
MGDL_INCLUDE = -I../include/
ROCKET_INCLUDE = -I../3rdparty/modules/rocket/

# Add them all to Compilation options
CXXFLAGS += $(GLUT_INCLUDE) $(MGDL_INCLUDE) $(ROCKET_INCLUDE)

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

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
