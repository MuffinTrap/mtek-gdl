
# Template project MacOS Makefile

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
# on MacOS this confuses the linker by making each file
# in rocket/ to appear twice
#cpp_src += $(wildcard rocket/*.cpp)
ROCKET_INCLUDE = -Irocket/
CXXFLAGS += $(ROCKET_INCLUDE)
# The rocket code of library is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# ############################################
# MacOS specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .out

# Homebrew include directories:
# This is for MacPorts. Alter to fit your system.
CXXFLAGS += -I/opt/local/include

# This is for homebrew
# Intel MacOS brew location
CXXFLAGS += -I/usr/local/include

# M1 MacOs brew location
CXXFLAGS += -I/opt/homebrew/include

# Exact locations on M1 mac
# CXXFLAGS += -I/opt/homebrew/Cellar/libsndfile/1.2.2/include -I/opt/homebrew/Cellar/openal-soft/1.23.1/include -I/opt/homebrew/Cellar/glm/1.0.1/include -I/opt/homebrew/Cellar/libpng/1.6.43/include

# Create a list of libraries that need to be linked
LDFLAGS = -lmgdl -lpng -lsndfile -Wno-unused-function

# MacOS need to tell to use OpenGL and GLUT instead of -lglut
LDFLAGS += -framework OpenGL -framework GLUT 

# Add MacPorts library path
LDFLAGS += -L/opt/local/lib

# Add homebrew library paths

# Intel MacOS
LDFLAGS += -L/usr/local/lib

# M1 MacOs
LDFLAGS += -L/opt/homebrew/lib

LDFLAGS += -framework OpenAL
# LDFLAGS = -L/opt/homebrew/Cellar/libsndfile/1.2.2/lib -L/opt/homebrew/Cellar/openal-soft/1.23.1/lib -L/opt/homebrew/opt/glm/lib -L/opt/homebrew/Cellar/libpng/1.6.43/lib


# Add mgdl library search directory and include
MGDL_DIR=$(HOME)/libmgdl
LDFLAGS += -L$(MGDL_DIR)
MGDL_INCLUDE = -I$(MGDL_DIR)


# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_mac.out

# ########################
# Common settings and targets

# Add them all to Compilation options
CXXFLAGS += $(MGDL_INCLUDE)

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.mo)

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
%.mo : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
