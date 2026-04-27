# Makefile used by all PC platforms
MGDL_DIR	=$(HOME)/libmgdl

SRC_DIRS = src

# Add mgdl library search directory and include
MGDL_INCLUDE	= -I$(MGDL_DIR)/include
CXXFLAGS += $(MGDL_INCLUDE)

# Optional: Add rocket files if variable ROCKET is set
ifdef ROCKET
SRC_DIRS += rocket
ROCKET_INCLUDE = -Irocket
CXXFLAGS += $(ROCKET_INCLUDE)
CXXFLAGS += -DMGDL_ROCKET
endif

# Optional: Use AngelScript either as script or compiled as c++
ifdef ANGEL

# TODO move to muffin-gdl
# Add AngelScript library directory
SRC_DIRS += dmon
CXXFLAGS += -Idmon
SRC_DIRS += angelscript
CXXFLAGS += -Iangelscript
LDFLAGS += -Langelscript/linux

ifeq ($(ANGEL), script)
CXXFLAGS += -DUSE_ANGEL_AS_SCRIPT
else ifeq ($(ANGEL), cpp)
CXXFLAGS += -DUSE_ANGEL_AS_CPP
SRC_DIRS += scripts
endif
endif


# Use find to gather all .cpp and .c files in SRC_DIRS
cpp_src = $(shell find $(SRC_DIRS) -name '*.cpp')


# Create a list of object files that make needs to
# process
OFILES	= $(cpp_src:.cpp=.pco)

ifeq ($(ANGEL), cpp)
# Find all angelscript files and add them to object files
angel_src = $(shell find $(SRC_DIRS) -name '*.cxx')
OFILES	+= $(angel_src:.cxx=.pco)

endif
