
# Create a list of libraries that need to be linked
# Link statically to mgdl and angelscript
LDFLAGS += -L$(MGDL_DIR)/lib
LDLIBS += -Wl,-Bstatic -lmgdl -langelscript -Wl,-Bdynamic -lopenal -lfreeglut -lglu32 -lopengl32 -lws2_32 -lm -Wno-unused-function
PLATFORM_NAME=msys2
CXXFLAGS += -DMGDL_PLATFORM=\"MSYS2\"
CXXFLAGS += -DMGDL_PLATFORM_MSYS2

TARGET_POSTFIX += .exe

# Add AngelScript library directory
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib

# Name the compiler
CXX = clang++
