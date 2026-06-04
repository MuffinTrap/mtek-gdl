
# Default build flags for all builds
CXXFLAGS += -Wall -Wextra -Wpedantic -std=c++11
# Don't complain about zstr
CXXFLAGS += -Wno-c99-extensions -Wno-c++20-designator

# Options for ufbx
CXXFLAGS += -DUFBX_REAL_IS_FLOAT

ifdef DEBUG
CXXFLAGS += -g
else
CXXFLAGS += -O3
endif

# Optional: Use AngelScript either as script or compiled as c++
ifdef ANGEL
# Read from $(ANGELSCRIPT_SDK) environment value directory
INCLUDES += -I$(ANGELSCRIPT_SDK)/angelscript/include
INCLUDES += -I$(ANGELSCRIPT_SDK)/add_on/

ifeq ($(ANGEL), script)
CXXFLAGS += -DUSE_ANGEL_AS_SCRIPT
else ifeq ($(ANGEL), cpp)
CXXFLAGS += -DUSE_ANGEL_AS_CPP
else
$(error Variable ANGEL must be defined either as 'script' or 'cpp' e.g. ANGEL=script)
endif # ifeq ANGEL
else
$(error Variable ANGEL must be defined either as 'script' or 'cpp' e.g. ANGEL=script)
endif # idef ANGEL
