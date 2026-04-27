
# Default build flags for all builds
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11
LDFLAGS =

# Options for ufbx and vector library selection
CXXFLAGS += -DUFBX_REAL_IS_FLOAT
CXXFLAGS += -DMGDL_USE_CCVECTOR

debug_flags = -g
#-Werror=unused-function
release_flags = -O3
