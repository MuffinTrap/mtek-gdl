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

# Read from $(ANGELSCRIPT_SDK) environment value directory
CXXFLAGS += -I$(ANGELSCRIPT_SDK)/angelscript/include
# Add AngelScript library directory
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib/linux

ifeq ($(ANGEL), script)
CXXFLAGS += -DUSE_ANGEL_AS_SCRIPT
else ifeq ($(ANGEL), cpp)
CXXFLAGS += -DUSE_ANGEL_AS_CPP
CXXFLAGS += -Iscripts
SRC_DIRS += scripts
else
$(error Variable ANGEL must be defined either as 'script' or 'cpp' e.g. ANGEL=script)
endif # ifeq ANGEL
endif # ifdef ANGEL


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
