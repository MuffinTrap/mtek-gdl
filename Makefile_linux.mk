# Build a linux static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(wildcard include/mgdl-pc/*.h)
HDR 	+= $(wildcard include/mgdl/*.h)
PCHDR 	:= include/mgdl.h

OBJ_DIR := obj_lnx

# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static
# Linux specific settings
CXX = clang++
#LD_FLAGS = -lpng -lsndfile -lopenal -lGL -lGLU -lglut -Wno-unused-function -z muldefs
GLUT_INCLUDE = -I/usr/include/GL/
MGDL_INCLUDE = -Iinclude/
CXX_FLAGS += $(GLUT_INCLUDE) $(MGDL_INCLUDE)

LIBDIR	:= lib/lnx

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
	rm -f $(OFILES) $(ARC)
	rm -r lib/pc

%.o : %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -c $< -o $@
	mv $@ $(OBJ_DIR)
