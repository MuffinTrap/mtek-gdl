# Makefile copied from riptutorial https://riptutorial.com/makefile/example/21376/building-from-different-source-folders-to-different-target-folders


ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

# ALL RULES
include ../build_rules/rules_all.mk

# Our Wii rules
include ../build_rules/rules_wii.mk


# GENERAL MAKEFILE

# Set project directory one level above of Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR)/..)
SOURCEDIR := $(PROJDIR)
BUILDDIR := $(PROJDIR)/build_$(PLATFORM_NAME)

# Name of the final executable
TARGET	:= $(notdir $(PROJDIR))$(TARGET_POSTFIX)


# Decide whether the commands will be shown or not
VERBOSE = TRUE

# Create the list of directories
DIRS = src
ifeq ($(ANGEL), cpp)
DIRS += scripts
endif

SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))

# Generate the GCC includes parameters by adding -I before each source folder
INCLUDES += $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.cpp sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))

# Define objects for all sources
OBJS = $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# OS specific part
ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    RMDIR = -RMDIR /S /Q
    MKDIR = -mkdir
    ERRIGNORE = 2>NUL || true
    SEP=\\
else
    RM = rm -rf
    RMDIR = rm -rf
    MKDIR = mkdir -p
    ERRIGNORE = 2>/dev/null
    SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
    HIDE =
else
    HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.cpp
	@echo Building Cpp $$@
	$(HIDE)$(CXX) -c $$(CXXFLAGS) $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

.PHONY: all clean directories

all: directories $(TARGET).dol
	@mv $(TARGET).dol boot.dol

$(TARGET).dol: $(TARGET).elf # Convert elf to dol

$(TARGET).elf: $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CXX) $(OBJS) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $(TARGET).elf

# Include dependencies
-include $(DEPS)

# Generate rules
$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(TARGET) $(ERRIGNORE)
	@echo Cleaning done !
