# Rules for all targets
include rules_all.mk
# Contains definitions shared by all PC platforms
include rules_pc.mk
# ############################################
# Linux specific settings
CXXFLAGS += -DMGDL_PLATFORM=\"Linux\"
CXXFLAGS += -DMGDL_PLATFORM_LINUX

# Address sanitizer reports leaks and crash reasons
# CXXFLAGS += -fsanitize=address

# Set Compiler
CXX = clang++
EXE_SUFFIX = .elf

# Include directories


# Link libraries

# Create a list of libraries that need to be linked
# Link statically to mgdl
LDFLAGS += -Wl,-Bstatic -lmgdl  -Wl,-Bdynamic -lopenal -lGL -lGLU -lglut -lm -Wno-unused-function -z muldefs
LDFLAGS += -langelscript

# Add mgdl library search directory
LDFLAGS += -L$(MGDL_DIR)/lib

# Executable is the same name as current directory +
# platform specific postfix
TARGET	:=	$(notdir $(CURDIR))_lnx.elf

# ########################
# Common settings and targets


.PHONY: all release debug

ifdef ROCKET
release: CXXFLAGS += -DSYNC_PLAYER
endif
release: CXXFLAGS += $(release_flags)
release: all

debug: CXXFLAGS += $(debug_flags)
debug: all

# When all OFILES have been processed, link them together
all : $(OFILES)
	@echo Linking...
	$(CXX) $(OFILES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)


# Remove obj directory, all object files and the target
clean:
	@echo clean ...
	@rm -fr $(OFILES) $(TARGET)

# For any .cpp file, create a object file with the same
# name.
%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.pco : %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
