include Makefile_pc.mk

# ######################################
# Windows specific settings
CXXFLAGS += -DMGDL_PLATFORM=\"Windows\ MSYS2\"
CXXFLAGS += -DMGDL_PLATFORM_MSYS2

# Set Compiler
CXX = clang++
EXE_SUFFIX = .exe

# Include directories













# Link libraries

# Create a list of libraries that need to be linked

# NOTE ! 
# C++ linking order is "last is first" and it does not read the
# list before it tries to link
# So the libraries not used by other libraries need to be last
# and mgdl needs to be first
LDFLAGS = -lmgdl -lopenal -lfreeglut -lglu32 -lopengl32 -lws2_32 -Wl,--allow-multiple-definition









# Add mgdl library search directory and include
LDFLAGS += -L$(MGDL_DIR)

# Executable is the same name as current directory +
# platform specific postfix
# Add numbers to the end if it crashes
# and you need to make a new one
TARGET	:=	$(notdir $(CURDIR))_msys2.exe

# END Windows specific

# ###################################
# Common settings and targets


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
