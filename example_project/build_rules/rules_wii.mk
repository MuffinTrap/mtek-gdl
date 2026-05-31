

MGDL_DIR	=$(DEVKITPRO)/../libmgdl
# Create a list of libraries that need to be linked
# Link statically to mgdl and angelscript
LDFLAGS += -L$(MGDL_DIR)/lib
LDLIBS += -lmgdl -lopengx -lglut -lGLU -lvalloc -lwiiuse -lpng -lz -lfat -lbte -lasnd -logc -lm
PLATFORM_NAME=wii
CXXFLAGS += -DMGDL_PLATFORM=\"Wii\"
CXXFLAGS += -DMGDL_PLATFORM_WII
CXXFLAGS += $(MACHDEP) # Add the MACHDEP from devkitppc/wii_rules

# No postfix because elf dot
TARGET_POSTFIX +=

# Add AngelScript library directory
ifdef ANGEL
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib/$(PLATFORM_NAME)
endif

# Name the compiler : named by DEVKITPRO
# CXX = GCC

# Add DEVKITPRO includes

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
PORTLIBS_PPC_INC	:=	$(PORTLIBS_PATH)/ppc/include
PORTLIBS_WII_INC	:=	$(PORTLIBS_PATH)/wii/include

INCLUDES += -I$(LIBOGC_INC) \
	-I$(PORTLIBS_PPC_INC) \
	-I$(PORTLIBS_WII_INC) \
	-I$(MGDL_DIR)/include

LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PORTLIBS_PPC_LIB	:=	$(DEVKITPRO)/portlibs/ppc/lib
PORTLIBS_WII_LIB	:=	$(DEVKITPRO)/portlibs/wii/lib

LDFLAGS += -L$(LIBOGC_LIB) \
	-L$(PORTLIBS_PPC_LIB) \
	-L$(PORTLIBS_WII_LIB)
