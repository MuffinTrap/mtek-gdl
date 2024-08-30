# Build a Windows static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
ARC 	:= lib$(LIB).a
HDR 	:= $(wildcard include/mgdl/*.h)
HDR 	+= $(wildcard include/mgdl-pc/*.h)
PCHDR 	:= include/mgdl.h
INSTALL_DIR = $(HOME)/libmgdl

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXX_FLAGS += $(MGDL_INCLUDE)

# Windows specific settings
CXX = g++
LIBDIR	:= lib/win
CXX_FLAGS += -DMGDL_PLATFORM_WINDOWS
# Windows file type
OFILES	:= $(CFILES:.cpp=.wo)

# Common part

.PHONY: all clean install

all : $(ARC)

$(ARC) : $(OFILES)
	@mkdir -p $(LIBDIR)
# Create static library
	@$(AR) rcs $(ARC) $(OBJ_FILES)
# Move static library
	@mv $(ARC) $(LIBDIR)
# Copy header files
	@cp $(HDR) $(LIBDIR)
# Copy main header
	@cp $(PCHDR) $(LIBDIR)
	@echo built library $(ARC)

# Installs to /home/user/libmgdl
install: $(ARC)
	@mkdir -p $(INSTALL_DIR)
	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)
	@cp $(HDR) $(INSTALL_DIR)
	@cp $(PCHDR) $(INSTALL_DIR)
	@echo installed to $(INSTALL_DIR):

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.o : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
