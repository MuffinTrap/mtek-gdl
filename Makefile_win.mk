# Build a linux static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(wildcard include/mgdl/*.h)
HDR 	+= $(wildcard include/mgdl-pc/*.h)
PCHDR 	:= include/mgdl.h

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXX_FLAGS += $(MGDL_INCLUDE)

# Windows specific settings
CXX = g++
LIBDIR	:= lib/win
OBJ_DIR := obj_win
CXX_FLAGS += -DMGDL_PLATFORM_WINDOWS

# Common part

.PHONY: all clean install

all : OBJ_FILES = $(wildcard $(OBJ_DIR)/*.o)
all : $(OFILES)
	mkdir -p $(LIBDIR)
# Create static library
	$(AR) rcs $(ARC) $(OBJ_FILES)
# Move static library
	mv $(ARC) $(LIBDIR)
# Copy header files
	cp $(HDR) $(LIBDIR)
# Copy main header
	cp $(PCHDR) $(LIBDIR)

clean :
	rm -f $(OBJ_DIR) $(ARC)
	rm -r $(LIB_DIR)

%.o : %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c $< -o $@
	mv $@ $(OBJ_DIR)
