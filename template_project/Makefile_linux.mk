
# Template project linux Makefile

SRC_DIRS := .

# Use only CXX to compile all files

# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src := $(shell find $(SRC_DIRS) -name '*.cpp')
cpp_src += $(wildcard ../3rdparty/modules/rocket/*.cpp)
OFILES	:= $(cpp_src:.cpp=.o)
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11

# For Debugging
CXX_FLAGS += -ggdb

# The rocket code of library is compiled in SYNC_PLAYER mode
# CXX_FLAGS += -DSYNC_PLAYER

# Linux specific settings
CXX = clang++
EXE_SUFFIX = .elf
LD_FLAGS = -lpng -lsndfile -lopenal -lGL -lGLU -lglut -lmgdl -Wno-unused-function -z muldefs
LD_FLAGS += -L../lib/pc
GLUT_INCLUDE = -I/usr/include/GL/
MGDL_INCLUDE = -I../include/
ROCKET_INCLUDE = -I../3rdparty/modules/rocket/
CXX_FLAGS += $(GLUT_INCLUDE) $(MGDL_INCLUDE) $(ROCKET_INCLUDE)

# Executable is the same name as current directory
TARGET	:=	$(notdir $(CURDIR))

.PHONY: all

all : $(OFILES)
	$(CXX) $(OFILES) $(CXX_FLAGS) $(LD_FLAGS) -o $(TARGET)_lnx.elf

clean:
	@echo clean ...
	@rm -fr $(OFILES) $(TARGET)_lnx.elf

%.o : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
