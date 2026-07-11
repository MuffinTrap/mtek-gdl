
# Create a list of libraries that need to be linked
# Link statically to mgdl and angelscript
LDFLAGS += -L$(MGDL_DIR)/lib
LDLIBS += -Wl,-Bstatic -lmgdl -langelscript -Wl,-Bdynamic -lopenal -lGL -lGLU -lglut -lm -Wno-unused-function -z muldefs
PLATFORM_NAME=linux
CXXFLAGS += -DMGDL_PLATFORM=\"Linux\"
CXXFLAGS += -DMGDL_PLATFORM_LINUX

TARGET_POSTFIX += _lnx.elf

# Add AngelScript library directory
ifdef ANGEL
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib
endif

# Name the compiler
CXX = clang++
