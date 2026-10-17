
# Create a list of libraries that need to be linked
# Link statically to mgdl and angelscript
LDFLAGS += -L$(MGDL_DIR)/lib

# List mgdl as first: first depends on the others
LDLIBS += -Wl,-Bstatic -lmgdl

# Optional AngelScript
LDLIBS += -Wl,-Bstatic -langelscript

# These are needed by mgdl
LDLIBS += -Wl,-Bdynamic -lopenal -lGL -lGLU -lglut -lm -Wno-unused-function -z muldefs

# Add AngelScript library directory
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib


PLATFORM_NAME=linux
CXXFLAGS += -DMGDL_PLATFORM=\"Linux\"
CXXFLAGS += -DMGDL_PLATFORM_LINUX

TARGET_POSTFIX += _lnx.elf


# Name the compiler
CXX = clang++
