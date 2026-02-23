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
UFBX_HDR	:= include/mgdl/ufbx/ufbx.h
CCVEC_HDR	:= include/mgdl/ccVector/ccVector.h
RANDOM_HDR	:= include/mgdl/wflcg/WFLCG_c.h
