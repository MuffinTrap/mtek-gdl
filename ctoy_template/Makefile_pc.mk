# Makefile used by all PC platforms
# NOTE This uses C compiler to work with Ziz code

SRC_DIRS := .

# Use only CC to compile all files

# Use find to gather all .c files in SRC_DIRS
c_src = $(shell find $(SRC_DIRS) -name '*.c')

ZIZ_DIR = # PATH TO ZIZ FOLDER , for example $(HOME)/Dev/Ziz
ZIZ_INCLUDE = $(ZIZ_DIR)/src
ZIZ_RESOURCES = $(ZIZ_DIR)/ressources
ZIZ_RESOURCES_SRC = $(ZIZ_RESOURCES)/src
ZIZ_RESOURCES_INCLUDE = $(ZIZ_DIR)/ressources/include
ZIZ_ASSETS = $(ZIZ_RESOURCES)/sourcefiles

# Add Ziz demo files
c_src += $(ZIZ_RESOURCES_SRC)/main.c $(ZIZ_RESOURCES_INCLUDE)/display.c

# Setup compilation options
# Windows does not have address sanitizer : stdc++ needed for std::vector in mgdl.a to work
CFLAGS = -Wnounused-function -Wall -Wextra -Wpedantic -std=c11 -lstdc++

# Extra compilation options
# #############################

# For Debugging
CFLAGS += -ggdb

CFLAGS += -DUFBX_REAL_IS_FLOAT
CFLAGS += -DMGDL_USE_CCVECTOR




# Rocket module
# Add rocket files to source
ROCKET_INCLUDE = -Irocket/
CFLAGS += $(ROCKET_INCLUDE)
# Needs string duplication in Rocket
CFLAGS += -DNEED_STRDUP

# Add Ziz
CFLAGS += -I$(ZIZ_INCLUDE)/
CFLAGS += -I$(ZIZ_MTCL)/ # For GL_macros.h
CFLAGS += -I$(ZIZ_RESOURCES_INCLUDE)/ # For gl_shapes.h
VPATH=$(ZIZ_INCLUDE)

# Add path to demo assets
CFLAGS += -DASSETS_PATH=\""$(ZIZ_ASSETS)/"\"

# Add mgdl library search directory and include
# Need to have mgdl as well because ufbx
MGDL_DIR	=$(HOME)/libmgdl
MGDL_INCLUDE	= -I$(MGDL_DIR)
CFLAGS += $(MGDL_INCLUDE)

# ##########################
# RELEASE BUILD
############################
# For optimization
# CFLAGS += -O3

# The rocket code of the release is compiled in SYNC_PLAYER mode
# CFLAGS += -DSYNC_PLAYER
