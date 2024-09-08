# Build a linux static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
ARC 	:= lib$(LIB).a
HDRS_X 	:= $(wildcard include/mgdl/*.h)
HDRS_PC	:= $(wildcard include/mgdl/pc/*.h)
HDRS := $(HDRS_X) $(HDRS_PC)
PCHDR 	:= include/mgdl.h
INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXX_FLAGS += $(MGDL_INCLUDE)

# MacOS specific settings
CXX = clang++
LIBDIR	:= lib/lnx
CXX_FLAGS += -DMGDL_PLATFORM_MAC
# Add MacPorts include path
CXX_FLAGS += -I/opt/local/include

# Intel MacOS homebrew location
CXX_FLAGS += -I/usr/local/include

# M1 MacOs homebrew location
CXX_FLAGS += -I/opt/homebrew/include

# Exact locations but should not be needed
# CXX_FLAGS += -I/opt/homebrew/Cellar/libsndfile/1.2.2/include -I/opt/homebrew/Cellar/openal-soft/1.23.1/include -I/opt/homebrew/Cellar/glm/1.0.1/include -I/opt/homebrew/Cellar/libpng/1.6.43/include

# Different file type for mac object files
OFILES	:= $(CFILES:.cpp=.mo)
# Common part

.PHONY: all clean install

all : $(ARC)

$(ARC): $(OFILES)
	@mkdir -p $(LIBDIR)
# Create static library
	@$(AR) rcs $(ARC) $(OFILES)
# Move static library
	@mv $(ARC) $(LIBDIR)
	@echo built library $(ARC)

# Installs to /home/user/libmgdl
install: $(ARC)
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/mgdl
	@mkdir -p $(INSTALL_DIR)/mgdl/pc
	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)
	@cp $(HDRS_X) $(INSTALL_DIR)/mgdl
	@cp $(HDRS_PC) $(INSTALL_DIR)/mgdl/pc
	@cp $(PCHDR) $(INSTALL_DIR)
	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.mo : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
