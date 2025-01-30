include Makefile_pc.mk

# ############################################
# Linux specific settings

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf

# Include directories













# Link libraries

# Create a list of libraries that need to be linked
LDFLAGS = -lmgdl -lpng -lsndfile -lopenal -lGL -lGLU -lglut -lvorbisidec -logg -Wno-unused-function -z muldefs















# Add mgdl library search directory and include
LDFLAGS += -L$(MGDL_DIR)

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

# ########################
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
