# This makefile is included by Makefile_pc and Makefile
# Library name
LIB 	:= mgdl
ARC 	:= lib$(LIB).a
DLL	:= lib$(LIB).so

# Own code
CXXFILES	= $(wildcard source/cross/*.cpp)
HDRS_X		:= $(wildcard include/mgdl/*.h)
LIBHDR		:= include/mgdl.h

# Embedded libraries
CXXFILES	+= $(wildcard include/mgdl/ufbx/*.cpp)
CXXFILES	+= $(wildcard source/stb/*.cpp)

# TODO All in source or all in include?
CXXFILES	+= $(wildcard source/angelscript/scriptbuilder/*.cpp)
CXXFILES	+= $(wildcard source/angelscript/scriptarray/*cpp)
CXXFILES	+= $(wildcard source/angelscript/scriptmath/*.cpp)
CXXFILES	+= $(wildcard source/angelscript/scriptmgdl/*.cpp)
CXXFILES	+= $(wildcard source/angelscript/scriptopengl/*.cpp)
CXXFLAGS 	+= -Isource/angelscript

UFBX_HDR	:= include/mgdl/ufbx/ufbx.h
CCVEC_HDR	:= include/mgdl/ccVector/ccVector.h
RANDOM_HDR	:= include/mgdl/wflcg/WFLCG_c.h
DMON_HDR		:= include/mgdl/dmon/dmon.h
RAYMATH_HDR	:= include/mgdl/raymath/raymath.h


# Silence zstr warnings
CXXFLAGS += -Wno-c99-extensions -Wno-c++20-designator

# UFBX settings
CXXFLAGS += -DUFBX_REAL_IS_FLOAT

# Add own include files so that #include <...> works
MGDL_INCLUDE = -Iinclude/
CXXFLAGS += $(MGDL_INCLUDE)

# Add AngelScript include
ifdef ANGELSCRIPT_SDK
CXXFLAGS += -I$(ANGELSCRIPT_SDK)/angelscript/include
CXXFLAGS += -I$(ANGELSCRIPT_SDK)/add_on
else
$(error Define ANGELSCRIPT_SDK to point to AngelScript/sdk on your system)
endif
