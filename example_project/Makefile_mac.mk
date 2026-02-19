include Makefile_pc.mk

# ############################################
# MacOS specific settings
CXXFLAGS += -DMGDL_PLATFORM=\"MacOS\"
CXXFLAGS += -DMGDL_PLATFORM_MAC

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

# Link libraries

# Create a list of libraries that need to be linked
LDFLAGS = -lmgdl -Wno-unused-function
# MacOS need to tell to use OpenGL, OpenAL and GLUT differently
LDFLAGS += -framework OpenGL -framework GLUT -framework OpenAL

# Add homebrew library paths
# MacPorts library path
LDFLAGS += -L/opt/local/lib
# Intel MacOS Homebrew
LDFLAGS += -L/usr/local/lib
# M1 MacOs Homebrew
LDFLAGS += -L/opt/homebrew/lib

# Exact locations if needed
# LDFLAGS = -L/opt/homebrew/Cellar/libsndfile/1.2.2/lib -L/opt/homebrew/Cellar/openal-soft/1.23.1/lib -L/opt/homebrew/opt/glm/lib -L/opt/homebrew/Cellar/libpng/1.6.43/lib


# Add mgdl library search directory and include
LDFLAGS += -L$(MGDL_DIR)/lib

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_mac.out

# ########################
# Common settings and targets

# Add them all to Compilation options

# Create a list of object files that make needs to
# process
OFILES	:= $(cpp_src:.cpp=.pco)

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
%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
