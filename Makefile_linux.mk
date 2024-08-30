# Build a linux static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
ARC 	:= lib$(LIB).a
HDRS_X 	:= $(wildcard include/mgdl/*.h)
HDRS_PC	:= $(wildcard include/mgdl-pc/*.h)
HDRS := $(HDRS_X) $(HDRS_PC)
PCHDR 	:= include/mgdl.h
INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXX_FLAGS += $(MGDL_INCLUDE)

# Linux specific settings
CXX = clang++
LIBDIR	:= lib/lnx
CXX_FLAGS += -DMGDL_PLATFORM_LINUX
# Different file type for linux object files
OFILES	:= $(CFILES:.cpp=.lo)
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
	@mkdir -p $(INSTALL_DIR)/mgdl-pc
	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)
	@cp $(HDRS_X) $(INSTALL_DIR)/mgdl
	@cp $(HDRS_PC) $(INSTALL_DIR)/mgdl-pc
	@cp $(PCHDR) $(INSTALL_DIR)
	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.lo : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
