# Build a linux static library

LIB 	:= mgdl
CFILES	= $(wildcard source/cross/*.cpp)
CFILES	+= $(wildcard source/pc/*.cpp)
OFILES	:= $(CFILES:.cpp=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(wildcard include/mgdl-pc/*.h)
HDR 	+= $(wildcard include/mgdl/*.h)
PCHDR 	:= include/mgdl.h


# Link everything statically
CXX_FLAGS = -Werror=unused-function -Wall -Wextra -Wpedantic -std=c++11 -static
# Linux specific settings
CXX = clang++
#LD_FLAGS = -lpng -lsndfile -lopenal -lGL -lGLU -lglut -Wno-unused-function -z muldefs
GLUT_INCLUDE = -I/usr/include/GL/
MGDL_INCLUDE = -Iinclude/
ROCKET_INCLUDE = -I3rdparty/libs-cross/
CXX_FLAGS += $(GLUT_INCLUDE) $(MGDL_INCLUDE) $(ROCKET_INCLUDE)

LIBDIR	:= lib/pc

.PHONY: all clean install

all : $(OFILES)
	mkdir -p $(LIBDIR)
# Create static library
	$(AR) rcs $(ARC) $(OFILES)
# Copy static library
	cp $(ARC) $(LIBDIR)
# Copy header files
	cp $(HDR) $(LIBDIR)
# Copy main header
	cp $(PCHDR) $(LIBDIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r lib/pc

#TODO
# Note: to install you need permissions to modify /opt/
# use the command $ sudo -E make install
# The -E flag uses the user's enviroment variables
# install :


%.o : %.cpp
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -c $< -o $@
