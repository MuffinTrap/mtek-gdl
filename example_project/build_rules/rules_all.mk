
# Default build flags for all builds
# zstr requires c++20
CXXFLAGS += -Wall -Wextra -Wpedantic -std=c++20
# Don't complain about zstr
CXXFLAGS += -Wno-c99-extensions -Wno-c++20-designator

# Options for ufbx
CXXFLAGS += -DUFBX_REAL_IS_FLOAT

ifdef DEBUG
CXXFLAGS += -g
else
CXXFLAGS += -O3
endif

# Read from $(ANGELSCRIPT_SDK) environment value directory
INCLUDES += -I$(ANGELSCRIPT_SDK)/angelscript/include
INCLUDES += -I$(ANGELSCRIPT_SDK)/add_on/
CXXFLAGS += -DUSE_ANGELSCRIPT
